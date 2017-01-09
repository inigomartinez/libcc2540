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
