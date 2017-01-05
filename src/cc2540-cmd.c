/*
 * Copyright (c) 2017 Iñigo Martínez <inigomartinez@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-3.0+
 */

#include <endian.h>
#include <errno.h>
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

int gap_evt_cmd_status (cc2540_t             *dev,
                        gap_evt_cmd_status_t *evt,
                        uint16_t              op_code);

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
    gap_evt_cmd_status_t status;

    memcpy (cmd.irk, irk, BT_IRK_LEN);
    memcpy (cmd.csrk, csrk, BT_CSRK_LEN);

    return gap_cmd (dev,
                    GAP_CMD_DEV_INIT, (const gap_cmd_t *) &cmd, sizeof (cmd),
                    &status);
}

CC2540_EXPORT int
gap_cmd_param_set (cc2540_t    *dev,
                   gap_param_t  param,
                   uint16_t     value) {
    gap_cmd_param_set_t cmd = {
        .param = param,
        .value = htole16 (value)
    };
    gap_evt_cmd_status_t status;

    return gap_cmd (dev,
                    GAP_CMD_PARAM_SET, (const gap_cmd_t *) &cmd, sizeof (cmd),
                    &status);
}

CC2540_EXPORT int
gap_cmd_param_get (cc2540_t    *dev,
                   gap_param_t  param,
                   uint16_t    *value) {
    int r;
    gap_cmd_param_get_t cmd = {
        .param = param
    };
    gap_evt_cmd_status_t status;

    if ((r = gap_cmd (dev,
                      GAP_CMD_PARAM_GET, (const gap_cmd_t *) &cmd, sizeof (cmd),
                      &status)) < 0)
        return r;

    *value = le16toh (get_unaligned (uint16_t, status.data));

    return 0;
}

CC2540_EXPORT int
gap_cmd (cc2540_t             *dev,
         uint16_t              op_code,
         const gap_cmd_t      *cmd,
         uint8_t               len,
         gap_evt_cmd_status_t *status) {
    hci_cmd_t hci = {
        .hci = {
            .type = HCI_TYPE_CMD,
            .op_code = htole16 (op_code),
            .len = len
        }
    };

    hci.cmd = *cmd;
    if (cc2540_write (dev, &hci, sizeof (hci_t) + len) == -1)
        return -errno;

    return gap_evt_cmd_status (dev, status, op_code);
}

CC2540_EXPORT int
gap_evt_dev_init_done (cc2540_t                *dev,
                       gap_evt_dev_init_done_t *evt) {
    static hci_evt_t hci;

    if (cc2540_read (dev, &hci, sizeof (hci_evt_t)) == -1)
        return -errno;

    if (le16toh (hci.op_code) != GAP_EVT_DEV_INIT_DONE)
        return -(errno = ENOMSG);

    if (cc2540_read (dev, evt, hci.data_len) == -1)
        return -errno;

    evt->data_pkt_len = le16toh (evt->data_pkt_len);
    __reverse (evt->addr, BT_ADDR_LEN);

    return 0;
}

int
gap_evt_cmd_status (cc2540_t             *dev,
                    gap_evt_cmd_status_t *evt,
                    uint16_t              op_code) {
    static hci_evt_t hci;

    if (cc2540_read (dev, &hci, sizeof (hci_evt_t)) == -1)
        return -errno;

    if (cc2540_read (dev, evt, hci.data_len) == -1)
        return -errno;

    evt->op_code = le16toh (evt->op_code);

    if (op_code != evt->op_code)
        return -(errno = ENOMSG);

    if (evt->status)
        return -(errno = EIO);

    return 0;
}
