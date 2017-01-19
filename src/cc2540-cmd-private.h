/*
 * Copyright (c) 2017 Iñigo Martínez <inigomartinez@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-3.0+
 */

#ifndef __CC2540_CMD_PRIVATE_H__
#define __CC2540_CMD_PRIVATE_H__

#include <endian.h>

#include "cc2540.h"
#include "cc2540-cmd.h"

CC2540_BEGIN_DECLS

#define HCI_INFO(op_code, len) \
    .hci = { \
        HCI_TYPE_CMD, \
        htole16 (op_code), \
        (len) \
    }

#define HCI_CMD_DEV_INIT(profile_role, max_scan_responses, sign_counter) \
    .cmd.dev_init = { \
        (profile_role), \
        (max_scan_responses), \
        {0}, \
        {0}, \
        htole32 (sign_counter) \
    }

#define HCI_CMD_DEV_ADDR_SET(addr_type) \
    .cmd.dev_addr_set = { \
        (addr_type), \
        {0}, \
    }

#define HCI_CMD_DEV_DISC(mode, active_scan, white_list) \
    .cmd.dev_disc = { \
        (mode), \
        (active_scan), \
        (white_list) \
    }

#define HCI_CMD_DISC_SET(evt_type, init_addr_type, channel_map, filter_policy) \
    .cmd.disc_set = { \
        (evt_type), \
        (init_addr_type), \
        {0}, \
        (channel_map), \
        (filter_policy) \
    }

#define HCI_CMD_ADV_SET(adv_type, data_len) \
    .cmd.adv_set = { \
        (adv_type), \
        (data_len), \
        {0}, \
    }

#define HCI_CMD_LINK_SET(high_duty, white_list, addr_type) \
    .cmd.link_set = { \
        (high_duty), \
        (white_list), \
        (addr_type), \
        {0}, \
    }

#define HCI_CMD_LINK_END(handle, reason) \
    .cmd.link_end = { \
        htole16 (handle), \
        (reason), \
    }

#define HCI_CMD_AUTH(handle, \
                     sec_io, sec_oob, sec_auth, sec_max_key_size, sec_key, \
                     pair_enable, pair_io, pair_oob, pair_auth, pair_max_key_size, pair_key) \
    .cmd.auth = { \
        htole16 (handle), \
        .sec = { \
            (sec_io), \
            (sec_oob), \
            {0}, \
            (sec_auth), \
            (sec_max_key_size), \
            (sec_key) \
        }, \
        .pair = { \
            (pair_enable), \
            (pair_io), \
            (pair_oob), \
            (pair_auth), \
            (pair_max_key_size), \
            (pair_key), \
        } \
    }

#define HCI_CMD_PASS_UPDATE(handle) \
    .cmd.pass_update = { \
        htole16 (handle), \
        {0}, \
    },

#define HCI_CMD_LINK_UPDATE(handle, min_interval, max_interval, latency, timeout) \
    .cmd.link_update = { \
        htole16 (handle), \
        htole16 (min_interval), \
        htole16 (max_interval), \
        htole16 (latency), \
        htole16 (timeout), \
    }

#define HCI_CMD_PARAM_SET(param, value) \
    .cmd.param_set = { \
        (param), \
        htole16 (value) \
    }

#define HCI_CMD_PARAM_GET(param) \
    .cmd.param_get = { \
        (param), \
    }

CC2540_END_DECLS

#endif /* __CC2540_CMD_PRIVATE_H__ */
