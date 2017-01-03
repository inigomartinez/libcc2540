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
