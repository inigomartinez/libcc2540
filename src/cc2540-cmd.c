/*
 * Copyright (c) 2017 Iñigo Martínez <inigomartinez@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-3.0+
 */

#include <endian.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "cc2540-private.h"
#include "cc2540.h"
#include "cc2540-cmd.h"

#define get_unaligned(type, ptr) \
__extension__ ({ \
    struct __attribute__((packed)) { \
        type __v; \
    } *__p = (__typeof__(__p)) (ptr); \
    __p->__v; \
})

void gap_evt_dev_init_done (hci_evt_t *evt);
void gap_evt_dev_disc      (hci_evt_t *evt);
void gap_evt_dev_info      (hci_evt_t *evt);
void gap_evt_cmd_status    (hci_evt_t *evt);

static struct {
    uint16_t  evt_code;
    void     (*parser) (hci_evt_t *evt);
} parsers[] = {
    {GAP_EVT_DEV_INIT_DONE, gap_evt_dev_init_done},
    {GAP_EVT_DEV_DISC,      gap_evt_dev_disc},
    {GAP_EVT_DEV_INFO,      gap_evt_dev_info},
    {GAP_EVT_CMD_STATUS,    gap_evt_cmd_status},
    {0}
};

static inline int parse (hci_evt_t *evt) {
    for (uint8_t n = 0; parsers[n].evt_code; n++) {
        if (parsers[n].evt_code == evt->evt_code) {
            parsers[n].parser (evt);
            return 0;
        }
    }
    return -(errno = ENOSYS);
}

static inline void __reverse (uint8_t *p, size_t len) {
    for (size_t i = 0, j = len - 1; i < j; i++, j--) {
        uint8_t v = p[i];
        p[i] = p[j];
        p[j] = v;
    }
}

CC2540_EXPORT int
gap_cmd_dev_init (cc2540_t      *dev,
                  gap_profile_t  profile_role,
                  uint8_t        max_scan_responses,
                  const uint8_t  irk[BT_IRK_LEN],
                  const uint8_t  csrk[BT_CSRK_LEN],
                  uint32_t       sign_counter) {
    gap_cmd_dev_init_t cmd = {
        .profile_role = profile_role,
        .max_scan_responses = max_scan_responses,
        .sign_counter = htole32 (sign_counter)
    };
    hci_evt_t evt;

    memcpy (cmd.irk, irk, BT_IRK_LEN);
    memcpy (cmd.csrk, csrk, BT_CSRK_LEN);

    return gap_cmd (dev,
                    GAP_CMD_DEV_INIT, GAP_CMD_T (&cmd), sizeof (cmd),
                    &evt);
}

CC2540_EXPORT int
gap_cmd_dev_disc (cc2540_t   *dev,
                  gap_scan_t  mode,
                  bool        active_scan,
                  bool        white_list) {
    gap_cmd_dev_disc_t cmd = {
        .mode = mode,
        .active_scan = active_scan,
        .white_list = white_list
    };
    hci_evt_t evt;

    return gap_cmd (dev,
                    GAP_CMD_DEV_DISC, GAP_CMD_T (&cmd), sizeof (cmd),
                    &evt);
}

CC2540_EXPORT int
gap_cmd_param_set (cc2540_t    *dev,
                   gap_param_t  param,
                   uint16_t     value) {
    gap_cmd_param_set_t cmd = {
        .param = param,
        .value = htole16 (value)
    };
    hci_evt_t evt;

    return gap_cmd (dev,
                    GAP_CMD_PARAM_SET, GAP_CMD_T (&cmd), sizeof (cmd),
                    &evt);
}

CC2540_EXPORT int
gap_cmd_param_get (cc2540_t    *dev,
                   gap_param_t  param,
                   uint16_t    *value) {
    int r;
    gap_cmd_param_get_t cmd = {
        .param = param
    };
    hci_evt_t evt;

    if ((r = gap_cmd (dev,
                      GAP_CMD_PARAM_GET, GAP_CMD_T (&cmd), sizeof (cmd),
                      &evt)) < 0)
        return r;

    *value = le16toh (get_unaligned (uint16_t, GAP_EVT_CMD_STATUS_T (&(evt.evt))->data));

    return 0;
}

CC2540_EXPORT int
gap_cmd (cc2540_t        *dev,
         uint16_t         op_code,
         const gap_cmd_t *cmd,
         uint8_t          len,
         hci_evt_t       *evt) {
    int r;
    hci_cmd_t hci = {
        .hci = {
            .type = HCI_TYPE_CMD,
            .op_code = htole16 (op_code),
            .len = len
        }
    };

    hci.cmd = *cmd;
    if (cc2540_write (dev, &hci, sizeof (hci.hci) + len) == -1)
        return -errno;

    if ((r = hci_evt (dev, evt)) < 0)
        return r;

    if (!HCI_EVT_IS (*evt, GAP_EVT_CMD_STATUS))
        return -(errno = ENOMSG);

    if (evt->evt.status)
        return -(errno = EIO);

    return 0;
}

CC2540_EXPORT int
hci_evt (cc2540_t  *dev,
         hci_evt_t *evt) {
    static hci_evt_info_t hci;

    if (cc2540_read (dev, &hci, sizeof (hci)) == -1)
        return -errno;

    evt->evt_code = le16toh (hci.op_code);
    if (cc2540_read (dev, &(evt->evt), hci.data_len) == -1)
        return -errno;

    return parse (evt);
}

void
gap_evt_dev_init_done (hci_evt_t *evt) {
    gap_evt_dev_init_done_t *data = GAP_EVT_DEV_INIT_DONE_T (&(evt->evt));

    data->data_pkt_len = le16toh (data->data_pkt_len);
    __reverse (data->addr, BT_ADDR_LEN);
}

void
gap_evt_dev_disc (hci_evt_t *evt) {
    gap_evt_dev_disc_t *data = GAP_EVT_DEV_DISC_T (&(evt->evt));

    for (uint8_t n = 0; n < data->num_devs; n++)
        __reverse (data->dev[n].addr, BT_ADDR_LEN);
}

void
gap_evt_dev_info (hci_evt_t *evt) {
    __reverse (GAP_EVT_DEV_INFO_T (&(evt->evt))->addr, BT_ADDR_LEN);
}

void
gap_evt_cmd_status (hci_evt_t *evt) {
    gap_evt_cmd_status_t *data = GAP_EVT_CMD_STATUS_T (&(evt->evt));

    data->op_code = le16toh (data->op_code);
}
