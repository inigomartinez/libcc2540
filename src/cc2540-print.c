/*
 * Copyright (c) 2017 Iñigo Martínez <inigomartinez@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-3.0+
 */

#include <stdint.h>
#include <stdio.h>

#include "cc2540-private.h"
#include "cc2540.h"
#include "cc2540-cmd.h"
#include "cc2540-print.h"

CC2540_EXPORT void
print_hci_evt_info (const hci_evt_info_t *evt) {
    printf ("HCI\n");
    printf ("type: 0x%02x\n", evt->type);
    printf ("evt_code: 0x%02x\n", evt->evt_code);
    printf ("data_len: 0x%02x\n", evt->data_len);
    printf ("op_code: 0x%04x\n", evt->op_code);
}

CC2540_EXPORT void
print_gap_evt_dev_init_done (const gap_evt_dev_init_done_t *evt) {
    printf ("GAP_DeviceInitDone\n");
    printf ("status: 0x%02x\n", evt->status);
    printf ("addr: %02x", evt->addr[0]);
    for (uint8_t n = 1; n < BT_ADDR_LEN; n++)
        printf (":%02x", evt->addr[n]);
    printf ("\n");
    printf ("data_pkt_len: %u\n", evt->data_pkt_len);
    printf ("num_data_pkts: %u\n", evt->num_data_pkts);
    printf ("irk: %02x", evt->irk[0]);
    for (uint8_t n = 1; n < BT_IRK_LEN; n++)
        printf (":%02x", evt->irk[n]);
    printf ("\n");
    printf ("csrk: %02x", evt->csrk[0]);
    for (uint8_t n = 1; n < BT_CSRK_LEN; n++)
        printf (":%02x", evt->csrk[n]);
    printf ("\n");
}

CC2540_EXPORT void
print_gap_evt_cmd_status (const gap_evt_cmd_status_t *evt) {
    printf ("CommandStatus\n");
    printf ("status: 0x%02x\n", evt->status);
    printf ("op_code: 0x%04x\n", evt->op_code);
    printf ("data_len: %u\n", evt->data_len);
    if (evt->data_len) {
        printf ("data: %02x", evt->data[0]);
        for (uint8_t n = 1; n < evt->data_len; ++n)
            printf (":%02x", evt->data[n]);
        printf ("\n");
    }
}
