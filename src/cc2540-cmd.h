/*
 * Copyright (c) 2017 Iñigo Martínez <inigomartinez@gmail.com>
 *
  SPDX-License-Identifier: LGPL-3.0+
 */

#ifndef __CC2540_CMD_H__
#define __CC2540_CMD_H__

#include <stdbool.h>
#include <stdint.h>

#include "cc2540.h"

CC2540_BEGIN_DECLS

#define BT_IRK_LEN      16
#define BT_CSRK_LEN     16
#define BT_LTK_LEN      16
#define BT_LTK_RAND_LEN 8
#define BT_ADDR_LEN     6

#define HCI_EVT_MAX_LEN UINT8_MAX

#define GAP_CMD_DEV_INIT      0xFE00
#define GAP_CMD_DEV_ADDR_SET  0xFE03
#define GAP_CMD_DEV_DISC      0xFE04
#define GAP_CMD_DEV_DISC_END  0xFE05
#define GAP_CMD_PARAM_SET     0xFE30
#define GAP_CMD_PARAM_GET     0xFE31

#define GAP_EVT_DEV_INIT_DONE 0x0600
#define GAP_EVT_DEV_DISC      0x0601
#define GAP_EVT_DEV_INFO      0x060D
#define GAP_EVT_CMD_STATUS    0x067F

#define GAP_MAX_SCAN_RESPONSES 0xFF

typedef enum {
    HCI_TYPE_CMD        = 0x01,
    HCI_TYPE_ASYNC_DATA = 0x02,
    HCI_TYPE_SYNC_DATA  = 0x03,
    HCI_TYPE_EVENT      = 0x04
} __attribute__((packed)) hci_type_t;

typedef enum {
    GAP_PROFILE_BROADCASTER = (1 << 0),
    GAP_PROFILE_OBSERVER    = (1 << 1),
    GAP_PROFILE_PERIPHERAL  = (1 << 2),
    GAP_PROFILE_CENTRAL     = (1 << 3)
} __attribute__((packed)) gap_profile_t;

typedef enum {
    GAP_SCAN_NON_DISC,
    GAP_SCAN_GEN,
    GAP_SCAN_LIM,
    GAP_SCAN_ALL
} __attribute__((packed)) gap_scan_t;

typedef enum {
    GAP_EVT_CONN_UNDIR,
    GAP_EVT_CONN_DIR,
    GAP_EVT_DISC_UNDIR,
    GAP_EVT_NO_CONN_UNDIR,
    GAP_EVT_SCAN_RESP
} __attribute__((packed)) gap_evt_t;

typedef enum {
    GAP_ADDR_PUBLIC,
    GAP_ADDR_STATIC,
    GAP_ADDR_PRIV_NON_RESOLV,
    GAP_ADDR_PRIV_RESOLV
} __attribute__((packed)) gap_addr_t;

typedef enum {
    TGAP_GEN_DISC_ADV_MIN,
    TGAP_LIM_ADV_TIMEOUT,
    TGAP_GEN_DISC_SCAN,
    TGAP_LIM_DISC_SCAN,
    TGAP_CONN_EST_ADV_TIMEOUT,
    TGAP_CONN_PARAM_TIMEOUT,
    TGAP_LIM_DISC_ADV_INT_MIN,
    TGAP_LIM_DISC_ADV_INT_MAX,
    TGAP_GEN_DISC_ADV_INT_MIN,
    TGAP_GEN_DISC_ADV_INT_MAX,
    TGAP_CONN_ADV_INT_MIN,
    TGAP_CONN_ADV_INT_MAX,
    TGAP_CONN_SCAN_INT,
    TGAP_CONN_SCAN_WIND,
    TGAP_CONN_HIGH_SCAN_INT,
    TGAP_CONN_HIGH_SCAN_WIND,
    TGAP_GEN_DISC_SCAN_INT,
    TGAP_GEN_DISC_SCAN_WIND,
    TGAP_LIM_DISC_SCAN_INT,
    TGAP_LIM_DISC_SCAN_WIND,
    TGAP_CONN_EST_ADV,
    TGAP_CONN_EST_INT_MIN,
    TGAP_CONN_EST_INT_MAX,
    TGAP_CONN_EST_SCAN_INT,
    TGAP_CONN_EST_SCAN_WIND,
    TGAP_CONN_EST_SUPERV_TIMEOUT,
    TGAP_CONN_EST_LATENCY,
    TGAP_CONN_EST_MIN_CE_LEN,
    TGAP_CONN_EST_MAX_CE_LEN,
    TGAP_PRIVATE_ADDR_INT,
    TGAP_CONN_PAUSE_CENTRAL,
    TGAP_CONN_PAUSE_PERIPHERAL,
    TGAP_SM_TIMEOUT,
    TGAP_SM_MIN_KEY_LEN,
    TGAP_SM_MAX_KEY_LEN,
    TGAP_FILTER_ADV_REPORTS,
    TGAP_SCAN_RSP_RSSI_MIN,
    TGAP_REJECT_CONN_PARAMS,
    // TESTING PARAMS
    TGAP_GAP_TESTCODE,
    TGAP_SM_TESTCODE,
    TGAP_GATT_TESTCODE = 100,
    TGAP_ATT_TESTCODE,
    TGAP_GGS_TESTCODE,
    TGAP_L2CAP_TESTCODE
} __attribute__((packed)) gap_param_t;

typedef struct {
    hci_type_t type;
    uint16_t   op_code;
    uint8_t    len;
} __attribute__((packed)) hci_cmd_info_t;

typedef struct {
    hci_type_t type;
    uint8_t    evt_code;
    uint8_t    data_len;
    uint16_t   op_code;
} __attribute__((packed)) hci_evt_info_t;

typedef struct {
    gap_profile_t profile_role;
    uint8_t       max_scan_responses;
    uint8_t       irk[BT_IRK_LEN];
    uint8_t       csrk[BT_CSRK_LEN];
    uint32_t      sign_counter;
} __attribute__((packed)) gap_cmd_dev_init_t;

typedef struct {
    gap_addr_t addr_type;
    uint8_t    addr[BT_ADDR_LEN];
} __attribute__((packed)) gap_cmd_dev_addr_set_t;

typedef struct {
    gap_scan_t mode;
    bool       active_scan;
    bool       white_list;
} __attribute__((packed)) gap_cmd_dev_disc_t;

typedef struct {
    gap_param_t param;
    uint16_t    value;
} __attribute__((packed)) gap_cmd_param_set_t;

typedef struct {
    gap_param_t param;
} __attribute__((packed)) gap_cmd_param_get_t;

typedef union {
    gap_cmd_dev_init_t     dev_init;
    gap_cmd_dev_addr_set_t dev_addr_set;
    gap_cmd_dev_disc_t     dev_disc;
    gap_cmd_param_set_t    param_set;
    gap_cmd_param_get_t    param_get;
} __attribute__((packed)) gap_cmd_t;

typedef struct {
    hci_cmd_info_t hci;
    gap_cmd_t      cmd;
} __attribute__((packed)) hci_cmd_t;

typedef struct {
    uint8_t  status;
    uint8_t  addr[BT_ADDR_LEN];
    uint16_t data_pkt_len;
    uint8_t  num_data_pkts;
    uint8_t  irk[BT_IRK_LEN];
    uint8_t  csrk[BT_CSRK_LEN];
} __attribute__((packed)) gap_evt_dev_init_done_t;

#define GAP_EVT_DEV_INIT_DONE_T(o) ((gap_evt_dev_init_done_t *) o)

typedef struct {
    uint8_t status;
    uint8_t num_devs;
    struct {
        gap_evt_t  evt_type;
        gap_addr_t addr_type;
        uint8_t    addr[BT_ADDR_LEN];
    } dev[];
} __attribute__((packed)) gap_evt_dev_disc_t;

#define GAP_EVT_DEV_DISC_T(o) ((gap_evt_dev_disc_t *) o)

typedef struct {
    uint8_t    status;
    gap_evt_t  evt_type;
    gap_addr_t addr_type;
    uint8_t    addr[BT_ADDR_LEN];
    int8_t     rssi;
    uint8_t    data_len;
    uint8_t    data[];
} __attribute__((packed)) gap_evt_dev_info_t;

#define GAP_EVT_DEV_INFO_T(o) ((gap_evt_dev_info_t *) o)

typedef struct {
    uint8_t  status;
    uint16_t op_code;
    uint8_t  data_len;
    uint8_t  data[];
} __attribute__((packed)) gap_evt_cmd_status_t;

#define GAP_EVT_CMD_STATUS_T(o) ((gap_evt_cmd_status_t *) o)

typedef struct {
    uint16_t  evt_code;
    struct {
        uint8_t status;
        uint8_t data[HCI_EVT_MAX_LEN - 1];
    } evt;
} __attribute__((packed)) hci_evt_t;

#define HCI_EVT_IS(evt, code) ((evt).evt_code == code)

int gap_cmd_dev_init     (cc2540_t        *dev,
                          gap_profile_t    profile_role,
                          uint8_t          max_scan_responses,
                          const uint8_t    irk[BT_IRK_LEN],
                          const uint8_t    csrk[BT_CSRK_LEN],
                          uint32_t         sign_counter);
int gap_cmd_dev_addr_set (cc2540_t        *dev,
                          gap_addr_t       type,
                          const uint8_t    addr[BT_ADDR_LEN]);
int gap_cmd_dev_disc     (cc2540_t        *dev,
                          gap_scan_t       mode,
                          bool             active_scan,
                          bool             white_list);
int gap_cmd_dev_disc_end (cc2540_t        *dev);
int gap_cmd_param_set    (cc2540_t        *dev,
                          gap_param_t      param,
                          uint16_t         value);
int gap_cmd_param_get    (cc2540_t        *dev,
                          gap_param_t      param,
                          uint16_t        *value);

int hci_cmd              (cc2540_t        *dev,
                          const hci_cmd_t *cmd,
                          hci_evt_t       *evt);
int hci_evt              (cc2540_t        *dev,
                          hci_evt_t       *evt);

CC2540_END_DECLS

#endif /* __CC2540_CMD_H__ */
