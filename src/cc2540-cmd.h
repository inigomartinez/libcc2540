/*
 * Copyright (c) 2017 Iñigo Martínez <inigomartinez@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-3.0+
 */

#ifndef __CC2540_CMD_H__
#define __CC2540_CMD_H__

#include <stdint.h>

CC2540_BEGIN_DECLS

#define BT_IRK_LEN      16
#define BT_CSRK_LEN     16
#define BT_LTK_LEN      16
#define BT_LTK_RAND_LEN 8
#define BT_ADDR_LEN     6

#define HCI_TYPE_CMD        0x01
#define HCI_TYPE_ASYNC_DATA 0x02
#define HCI_TYPE_SYNC_DATA  0x03
#define HCI_TYPE_EVENT      0x04

#define GAP_CMD_DEV_INIT      0xFE00

#define GAP_EVT_DEV_INIT_DONE 0x0600

#define GAP_PROFILE_BROADCASTER 0x01
#define GAP_PROFILE_OBSERVER    0x02
#define GAP_PROFILE_PERIPHERAL  0x04
#define GAP_PROFILE_CENTRAL     0x08

#define GAP_MAX_SCAN_RESPONSES    0xFF

typedef struct {
    uint8_t  type;
    uint16_t op_code;
    uint8_t  len;
} __attribute__((packed)) hci_t;

typedef struct {
    uint8_t  type;
    uint8_t  event_code;
    uint8_t  data_len;
    uint16_t op_code;
} __attribute__((packed)) hci_evt_t;

typedef struct {
    uint8_t  profile_role;
    uint8_t  max_scan_responses;
    uint8_t  irk[BT_IRK_LEN];
    uint8_t  csrk[BT_CSRK_LEN];
    uint32_t sign_counter;
} __attribute__((packed)) gap_cmd_dev_init_t;

typedef union {
    gap_cmd_dev_init_t dev_init;
} __attribute__((packed)) gap_cmd_t;

typedef struct {
    hci_t     hci;
    gap_cmd_t cmd;
} __attribute__((packed)) hci_cmd_t;

typedef struct {
    uint8_t  status;
    uint8_t  addr[BT_ADDR_LEN];
    uint16_t data_pkt_len;
    uint8_t  num_data_pkts;
    uint8_t  irk[BT_IRK_LEN];
    uint8_t  csrk[BT_CSRK_LEN];
} __attribute__((packed)) gap_evt_dev_init_done_t;

typedef struct {
    uint8_t  status;
    uint16_t op_code;
    uint8_t  data_len;
    uint8_t  data[];
} __attribute__((packed)) gap_evt_cmd_status_t;

int gap_cmd               (cc2540_t                *dev,
                           uint16_t                 op_code,
                           const gap_cmd_t         *cmd,
                           uint8_t                  len,
                           gap_evt_cmd_status_t    *status);
int gap_evt_dev_init_done (cc2540_t                *dev,
                           gap_evt_dev_init_done_t *evt);

CC2540_END_DECLS

#endif /* __CC2540_CMD_H__ */
