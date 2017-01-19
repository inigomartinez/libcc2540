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
#include "cc2540-cmd-private.h"
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
void gap_evt_disc_set_done (hci_evt_t *evt);
void gap_evt_link_set      (hci_evt_t *evt);
void gap_evt_link_end      (hci_evt_t *evt);
void gap_evt_link_update   (hci_evt_t *evt);
void gap_evt_auth          (hci_evt_t *evt);
void gap_evt_dev_info      (hci_evt_t *evt);
void gap_evt_cmd_status    (hci_evt_t *evt);

static struct {
    uint16_t  evt_code;
    void     (*parser) (hci_evt_t *evt);
} parsers[] = {
    {GAP_EVT_DEV_INIT_DONE, gap_evt_dev_init_done},
    {GAP_EVT_DEV_DISC,      gap_evt_dev_disc},
    {GAP_EVT_ADV_SET_DONE,  NULL},
    {GAP_EVT_DISC_SET_DONE, gap_evt_disc_set_done},
    {GAP_EVT_DISC_END,      NULL},
    {GAP_EVT_LINK_SET,      gap_evt_link_set},
    {GAP_EVT_LINK_END,      gap_evt_link_end},
    {GAP_EVT_LINK_UPDATE,   gap_evt_link_update},
    {GAP_EVT_AUTH,          gap_evt_auth},
    {GAP_EVT_DEV_INFO,      gap_evt_dev_info},
    {GAP_EVT_CMD_STATUS,    gap_evt_cmd_status},
    {0}
};

static inline int parse (hci_evt_t *evt) {
    for (uint8_t n = 0; parsers[n].evt_code; n++) {
        if (HCI_EVT_IS (*evt, parsers[n].evt_code)) {
            if (parsers[n].parser)
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
    hci_cmd_t hci = {
        HCI_INFO (GAP_CMD_DEV_INIT, sizeof (gap_cmd_dev_init_t)),
        HCI_CMD_DEV_INIT (profile_role, max_scan_responses, sign_counter)
    };
    hci_evt_t evt;

    memcpy (hci.cmd.dev_init.irk, irk, BT_IRK_LEN);
    memcpy (hci.cmd.dev_init.csrk, csrk, BT_CSRK_LEN);

    return hci_cmd (dev, &hci, &evt);
}

CC2540_EXPORT int
gap_cmd_dev_addr_set (cc2540_t      *dev,
                      gap_addr_t     addr_type,
                      const uint8_t  addr[BT_ADDR_LEN]) {
    hci_cmd_t hci = {
        HCI_INFO (GAP_CMD_DEV_ADDR_SET, sizeof (gap_cmd_dev_addr_set_t)),
        HCI_CMD_DEV_ADDR_SET (addr_type)
    };
    hci_evt_t evt;

    memcpy (hci.cmd.dev_addr_set.addr, addr, BT_ADDR_LEN);
    __reverse (hci.cmd.dev_addr_set.addr, BT_ADDR_LEN);

    return hci_cmd (dev, &hci, &evt);
}

CC2540_EXPORT int
gap_cmd_dev_disc (cc2540_t   *dev,
                  gap_scan_t  mode,
                  bool        active_scan,
                  bool        white_list) {
    hci_cmd_t hci = {
        HCI_INFO (GAP_CMD_DEV_DISC, sizeof (gap_cmd_dev_disc_t)),
        HCI_CMD_DEV_DISC (mode, active_scan, white_list)
    };
    hci_evt_t evt;

    return hci_cmd (dev, &hci, &evt);
}

CC2540_EXPORT int
gap_cmd_dev_disc_end (cc2540_t *dev) {
    hci_cmd_t hci = {
        HCI_INFO (GAP_CMD_DEV_DISC_END, 0)
    };
    hci_evt_t evt;

    return hci_cmd (dev, &hci, &evt);
}

CC2540_EXPORT int
gap_cmd_disc_set (cc2540_t      *dev,
                  gap_evt_t      evt_type,
                  gap_addr_t     init_addr_type,
                  const uint8_t  init_addr[BT_ADDR_LEN],
                  gap_channel_t  channel_map,
                  gap_filter_t   filter_policy) {
    hci_cmd_t hci = {
        HCI_INFO (GAP_CMD_DISC_SET, sizeof (gap_cmd_disc_set_t)),
        HCI_CMD_DISC_SET (evt_type, init_addr_type, channel_map, filter_policy)
    };
    hci_evt_t evt;

    memcpy (hci.cmd.disc_set.init_addr, init_addr, BT_ADDR_LEN);

    return hci_cmd (dev, &hci, &evt);
}

CC2540_EXPORT int
gap_cmd_adv_set (cc2540_t      *dev,
                 gap_adv_t      adv_type,
                 uint8_t        data_len,
                 const uint8_t  data[]) {
    uint8_t len = (data_len <= GAP_ADV_MAX_LEN ? data_len : GAP_ADV_MAX_LEN);
    hci_cmd_t hci = {
        HCI_INFO (GAP_CMD_ADV_SET, sizeof (adv_type) + sizeof (data_len) + len),
        HCI_CMD_ADV_SET (adv_type, len)
    };
    hci_evt_t evt;

    memcpy (hci.cmd.adv_set.data, data, len);

    return hci_cmd (dev, &hci, &evt);
}

CC2540_EXPORT int
gap_cmd_disc_end (cc2540_t *dev) {
    hci_cmd_t hci = {
        HCI_INFO (GAP_CMD_DISC_END, 0)
    };
    hci_evt_t evt;

    return hci_cmd (dev, &hci, &evt);
}

CC2540_EXPORT int
gap_cmd_link_set (cc2540_t      *dev,
                  bool           high_duty,
                  bool           white_list,
                  gap_addr_t     addr_type,
                  const uint8_t  addr[BT_ADDR_LEN]) {
    hci_cmd_t hci = {
        HCI_INFO (GAP_CMD_LINK_SET, sizeof (gap_cmd_link_set_t)),
        HCI_CMD_LINK_SET(high_duty, white_list, addr_type)
    };
    hci_evt_t evt;

    memcpy (hci.cmd.link_set.addr, addr, BT_ADDR_LEN);
    __reverse (hci.cmd.link_set.addr, BT_ADDR_LEN);

    return hci_cmd (dev, &hci, &evt);
}

CC2540_EXPORT int
gap_cmd_link_end (cc2540_t     *dev,
                  uint16_t      handle,
                  gap_reason_t  reason) {
    hci_cmd_t hci = {
        HCI_INFO (GAP_CMD_LINK_END, sizeof (gap_cmd_link_end_t)),
        HCI_CMD_LINK_END(handle, reason)
    };
    hci_evt_t evt;

    return hci_cmd (dev, &hci, &evt);
}

CC2540_EXPORT int
gap_cmd_auth (cc2540_t   *dev,
              uint16_t    handle,
              gap_io_t    sec_io,
              bool        sec_oob,
              uint8_t     sec_oob_init[BT_LTK_LEN],
              gap_auth_t  sec_auth,
              uint8_t     sec_max_key_size,
              gap_key_t   sec_key,
              bool        pair_enable,
              gap_io_t    pair_io,
              bool        pair_oob,
              gap_auth_t  pair_auth,
              uint8_t     pair_max_key_size,
              gap_key_t   pair_key) {
    hci_cmd_t hci = {
        HCI_INFO (GAP_CMD_AUTH, sizeof (gap_cmd_auth_t)),
        HCI_CMD_AUTH(handle,
                     sec_io, sec_oob, sec_auth, sec_max_key_size, sec_key,
                     pair_enable, pair_io, pair_oob, pair_auth, pair_max_key_size, pair_key)
    };
    hci_evt_t evt;

    memcpy (hci.cmd.auth.sec.oob_init, sec_oob_init, BT_LTK_LEN);

    return hci_cmd (dev, &hci, &evt);
}

CC2540_EXPORT int
gap_cmd_pass_update (cc2540_t   *dev,
                     uint16_t    handle,
                     const char  pass[GAP_PASS_STR_LEN]) {
    hci_cmd_t hci = {
        HCI_INFO (GAP_CMD_PASS_UPDATE, sizeof (gap_cmd_pass_update_t)),
        HCI_CMD_PASS_UPDATE(handle)
    };
    hci_evt_t evt;

    memcpy (hci.cmd.pass_update.pass, pass, GAP_PASS_LEN);

    return hci_cmd (dev, &hci, &evt);
}

CC2540_EXPORT int
gap_cmd_slave_sec (cc2540_t   *dev,
                   uint16_t    handle,
                   gap_auth_t  auth) {
    hci_cmd_t hci = {
        HCI_INFO (GAP_CMD_SLAVE_SEC, sizeof (gap_cmd_slave_sec_t)),
        HCI_CMD_SLAVE_SEC(handle, auth)
    };
    hci_evt_t evt;

    return hci_cmd (dev, &hci, &evt);
}

CC2540_EXPORT int
gap_cmd_link_update (cc2540_t *dev,
                     uint16_t  handle,
                     uint16_t  min_interval,
                     uint16_t  max_interval,
                     uint16_t  latency,
                     uint16_t  timeout) {
    hci_cmd_t hci = {
        HCI_INFO (GAP_CMD_LINK_UPDATE, sizeof (gap_cmd_link_update_t)),
        HCI_CMD_LINK_UPDATE(handle, min_interval, max_interval, latency, timeout)
    };
    hci_evt_t evt;

    return hci_cmd (dev, &hci, &evt);
}

CC2540_EXPORT int
gap_cmd_param_set (cc2540_t    *dev,
                   gap_param_t  param,
                   uint16_t     value) {
    hci_cmd_t hci = {
        HCI_INFO (GAP_CMD_PARAM_SET, sizeof (gap_cmd_param_set_t)),
        HCI_CMD_PARAM_SET (param, value)
    };
    hci_evt_t evt;

    return hci_cmd (dev, &hci, &evt);
}

CC2540_EXPORT int
gap_cmd_param_get (cc2540_t    *dev,
                   gap_param_t  param,
                   uint16_t    *value) {
    int r;
    hci_cmd_t hci = {
        HCI_INFO (GAP_CMD_PARAM_GET, sizeof (gap_cmd_param_get_t)),
        HCI_CMD_PARAM_GET (param)
    };
    hci_evt_t evt;

    if ((r = hci_cmd (dev, &hci, &evt)) < 0)
        return r;

    *value = le16toh (get_unaligned (uint16_t, GAP_EVT_CMD_STATUS_T (&(evt.evt))->data));

    return 0;
}

CC2540_EXPORT int
hci_cmd (cc2540_t        *dev,
         const hci_cmd_t *cmd,
         hci_evt_t       *evt) {
    int r;
    if (cc2540_write (dev, cmd, sizeof (cmd->hci) + cmd->hci.len) == -1)
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
gap_evt_disc_set_done (hci_evt_t *evt) {
    gap_evt_disc_set_done_t *data = GAP_EVT_DISC_SET_DONE_T (&(evt->evt));

    data->interval = le16toh (data->interval);
}

void
gap_evt_link_set (hci_evt_t *evt) {
    gap_evt_link_set_t *data = GAP_EVT_LINK_SET_T (&(evt->evt));

    __reverse (data->addr, BT_ADDR_LEN);
    data->handle = le16toh (data->handle);
    data->interval = le16toh (data->interval);
    data->latency = le16toh (data->latency);
    data->timeout = le16toh (data->timeout);
}

void
gap_evt_link_end (hci_evt_t *evt) {
    gap_evt_link_end_t *data = GAP_EVT_LINK_END_T (&(evt->evt));

    data->handle = le16toh (data->handle);
}

void
gap_evt_link_update (hci_evt_t *evt) {
    gap_evt_link_update_t *data = GAP_EVT_LINK_UPDATE_T (&(evt->evt));

    data->handle = le16toh (data->handle);
    data->interval = le16toh (data->interval);
    data->latency = le16toh (data->latency);
    data->timeout = le16toh (data->timeout);
}

void
gap_evt_auth (hci_evt_t *evt) {
    gap_evt_auth_t *data = GAP_EVT_AUTH_T (&(evt->evt));
    size_t data_size = sizeof (*data);

    data->handle = le16toh (data->handle);

    if (data->sec_info_status) {
        data->sec_info.div = le16toh (data->sec_info.div);
    } else {
        memmove (&(data->dev_sec_info_status), &(data->sec_info),
                 data_size - offsetof (gap_evt_auth_t, dev_sec_info_status));
    }

    if (data->dev_sec_info_status) {
        data->dev_sec_info.div = le16toh (data->dev_sec_info.div);
    } else {
        memmove (&(data->ident_info_status), &(data->dev_sec_info),
                 data_size - offsetof (gap_evt_auth_t, ident_info_status));
    }

    if (data->ident_info_status) {
        __reverse (data->ident_info.addr, BT_ADDR_LEN);
    } else {
        memmove (&(data->sign_info_status), &(data->ident_info),
                 data_size - offsetof (gap_evt_auth_t, sign_info_status));
    }

    if (data->sign_info_status)
        data->sign_info.sign_counter = le16toh (data->sign_info.sign_counter);
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
