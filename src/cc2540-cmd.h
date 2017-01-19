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
#define GAP_CMD_DISC_SET      0xFE06
#define GAP_CMD_ADV_SET       0xFE07
#define GAP_CMD_DISC_END      0xFE08
#define GAP_CMD_LINK_SET      0xFE09
#define GAP_CMD_LINK_END      0xFE0A
#define GAP_CMD_AUTH          0xFE0B
#define GAP_CMD_PASS_UPDATE   0xFE0C
#define GAP_CMD_LINK_UPDATE   0xFE11
#define GAP_CMD_PARAM_SET     0xFE30
#define GAP_CMD_PARAM_GET     0xFE31

#define GAP_EVT_DEV_INIT_DONE 0x0600
#define GAP_EVT_DEV_DISC      0x0601
#define GAP_EVT_ADV_SET_DONE  0x0602
#define GAP_EVT_DISC_SET_DONE 0x0603
#define GAP_EVT_DISC_END      0x0604
#define GAP_EVT_LINK_SET      0x0605
#define GAP_EVT_LINK_END      0x0606
#define GAP_EVT_LINK_UPDATE   0x0607
#define GAP_EVT_AUTH          0x060A
#define GAP_EVT_DEV_INFO      0x060D
#define GAP_EVT_CMD_STATUS    0x067F

#define GAP_ADV_MAX_LEN        0x20

#define GAP_MAX_SCAN_RESPONSES 0xFF

#define GAP_HANDLE_LINK 0xFFFE
#define GAP_HANDLE_ALL  0xFFFF

#define GAP_INTERVAL_TO_MSEC(v) ((v) * 1.25f)
#define GAP_TIMEOUT_TO_MSEC(v)  ((v) * 10)
#define GAP_MSEC_TO_INTERVAL(v) ((v) / 1.25f)
#define GAP_MSEC_TO_TIMEOUT(v)  ((v) / 10)

#define GAP_INTERVAL_MIN 0x0006
#define GAP_INTERVAL_MAX 0x0C80

#define GAP_LATENCY_MIN  0x0000
#define GAP_LATENCY_MAX  0x01F3

#define GAP_TIMEOUT_MIN  0x000A
#define GAP_TIMEOUT_MAX  0x0C80

#define GAP_PASS_LEN     6
#define GAP_PASS_STR_LEN (GAP_PASS_LEN + 1)

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
    GAP_CHANNEL_37 = (1 << 0),
    GAP_CHANNEL_38 = (1 << 1),
    GAP_CHANNEL_39 = (1 << 2)
} __attribute__((packed)) gap_channel_t;

typedef enum {
    GAP_FILTER_SCAN_ALL_CONN_ALL,
    GAP_FILTER_SCAN_LIST_CONN_ALL,
    GAP_FILTER_SCAN_ALL_CONN_LIST,
    GAP_FILTER_SCAN_LIST_CONN_LIST
} __attribute__((packed)) gap_filter_t;

typedef enum {
    GAP_ADV_SCAN_RSP,
    GAP_ADV_RAW
} __attribute__((packed)) gap_adv_t;

typedef enum {
    GAP_CLOCK_500,
    GAP_CLOCK_250,
    GAP_CLOCK_150,
    GAP_CLOCK_100,
    GAP_CLOCK_75,
    GAP_CLOCK_50,
    GAP_CLOCK_30,
    GAP_CLOCK_20
} __attribute__((packed)) gap_clock_t;

typedef enum {
    GAP_REASON_AUTH_FAIL                  = 0x05,
    GAP_REASON_TIMEOUT                    = 0x08,
    GAP_REASON_PEER                       = 0x13,
    GAP_REASON_PEER_LOW_RES               = 0x14,
    GAP_REASON_PEER_POWER_OFF             = 0x15,
    GAP_REASON_HOST                       = 0x16,
    GAP_REASON_PEER_NO_FEATURE            = 0x1A,
    GAP_REASON_CTRL_PACKET_TIMEOUT        = 0x22,
    GAP_REASON_CTRL_PACKET_INSTANT_PASSED = 0x28,
    GAP_REASON_UNIT_KEY_NOT_SUPPORTED     = 0x29,
    GAP_REASON_BAD_CONN_INTERVAL          = 0x3B,
    GAP_REASON_MIC_FAIL                   = 0x3D
} __attribute__((packed)) gap_reason_t;

typedef enum {
    GAP_IO_DISPLAY_ONLY,
    GAP_IO_DISPLAY_YN,
    GAP_IO_KEYB_ONLY,
    GAP_IO_NO_IO,
    GAP_IO_KEYB_DISPLAY
} __attribute__((packed)) gap_io_t;

typedef enum {
    GAP_AUTH_BONDING,
    GAP_AUTH_MITM_PROT = 2,
    GAP_AUTH_LE_SEC,
} __attribute__((packed)) gap_auth_t;

typedef enum {
    GAP_KEY_SLAVE_ENC    = (1 << 0),
    GAP_KEY_SLAVE_IDENT  = (1 << 1),
    GAP_KEY_SLAVE_SIGN   = (1 << 2),
    GAP_KEY_SLAVE_LTK    = (1 << 3),
    GAP_KEY_MASTER_ENC   = (1 << 4),
    GAP_KEY_MASTER_IDENT = (1 << 5),
    GAP_KEY_MASTER_SIGN  = (1 << 6),
    GAP_KEY_MASTER_LTK   = (1 << 7)
} __attribute__((packed)) gap_key_t;

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
    uint8_t  ltk_size;
    uint8_t  ltk[BT_LTK_LEN];
    uint16_t div;
    uint8_t  rand[BT_LTK_RAND_LEN];
} __attribute__((packed)) gap_sec_info_t;

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
    gap_evt_t     evt_type;
    gap_addr_t    init_addr_type;
    uint8_t       init_addr[BT_ADDR_LEN];
    gap_channel_t channel_map;
    gap_filter_t  filter_policy;
} __attribute__((packed)) gap_cmd_disc_set_t;

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
    gap_adv_t adv_type;
    uint8_t   data_len;
    uint8_t   data[GAP_ADV_MAX_LEN];
} __attribute__((packed)) gap_cmd_adv_set_t;

typedef struct {
    bool       high_duty;
    bool       white_list;
    gap_addr_t addr_type;
    uint8_t    addr[BT_ADDR_LEN];
} __attribute__((packed)) gap_cmd_link_set_t;

typedef struct {
    uint16_t     handle;
    gap_reason_t reason;
} __attribute__((packed)) gap_cmd_link_end_t;

typedef struct {
    uint16_t     handle;
    struct {
        gap_io_t   io;
        bool       oob;
        uint8_t    oob_init[BT_LTK_LEN];
        gap_auth_t auth;
        uint8_t    max_key_size;
        gap_key_t  key;
    } sec;
    struct {
        bool       enable;
        gap_io_t   io;
        bool       oob;
        gap_auth_t auth;
        uint8_t    max_key_size;
        gap_key_t  key;
    } pair;
} __attribute__((packed)) gap_cmd_auth_t;

typedef struct {
    uint16_t handle;
    uint8_t  pass[GAP_PASS_LEN];
} __attribute__((packed)) gap_cmd_pass_update_t;

typedef struct {
    uint16_t handle;
    uint16_t min_interval;
    uint16_t max_interval;
    uint16_t latency;
    uint16_t timeout;
} __attribute__((packed)) gap_cmd_link_update_t;

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
    gap_cmd_disc_set_t     disc_set;
    gap_cmd_adv_set_t      adv_set;
    gap_cmd_link_set_t     link_set;
    gap_cmd_link_end_t     link_end;
    gap_cmd_auth_t         auth;
    gap_cmd_pass_update_t  pass_update;
    gap_cmd_link_update_t  link_update;
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
    uint8_t   status;
    gap_adv_t adv_type;
} __attribute__((packed)) gap_evt_adv_set_done_t;

#define GAP_EVT_ADV_SET_DONE_T(o) ((gap_evt_adv_set_done_t *) o)

typedef struct {
    uint8_t  status;
    uint16_t interval;
} __attribute__((packed)) gap_evt_disc_set_done_t;

#define GAP_EVT_DISC_SET_DONE_T(o) ((gap_evt_disc_set_done_t *) o)

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
    uint8_t       status;
    gap_addr_t    addr_type;
    uint8_t       addr[BT_ADDR_LEN];
    uint16_t      handle;
    gap_profile_t role;
    uint16_t      interval;
    uint16_t      latency;
    uint16_t      timeout;
    gap_clock_t   clock_accuracy;
} __attribute__((packed)) gap_evt_link_set_t;

#define GAP_EVT_LINK_SET_T(o) ((gap_evt_link_set_t *) o)

typedef struct {
    uint8_t      status;
    uint16_t     handle;
    gap_reason_t reason;
} __attribute__((packed)) gap_evt_link_end_t;

#define GAP_EVT_LINK_END_T(o) ((gap_evt_link_end_t *) o)

typedef struct {
    uint8_t  status;
    uint16_t handle;
    uint16_t interval;
    uint16_t latency;
    uint16_t timeout;
} __attribute__((packed)) gap_evt_link_update_t;

#define GAP_EVT_LINK_UPDATE_T(o) ((gap_evt_link_update_t *) o)

typedef struct {
    uint8_t        status;
    uint16_t       handle;
    gap_auth_t     auth;
    bool           sec_info_status;
    gap_sec_info_t sec_info;
    bool           dev_sec_info_status;
    gap_sec_info_t dev_sec_info;
    bool           ident_info_status;
    struct {
        uint8_t irk[BT_IRK_LEN];
        uint8_t addr[BT_ADDR_LEN];
    } ident_info;
    bool           sign_info_status;
    struct {
        uint8_t  irk[BT_IRK_LEN];
        uint16_t sign_counter;
    } sign_info;
} __attribute__((packed)) gap_evt_auth_t;

#define GAP_EVT_AUTH_T(o) ((gap_evt_auth_t *) o)

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
int gap_cmd_disc_set     (cc2540_t        *dev,
                          gap_evt_t        evt_type,
                          gap_addr_t       init_addr_type,
                          const uint8_t    init_addr[BT_ADDR_LEN],
                          gap_channel_t    channel_map,
                          gap_filter_t     filter_policy);
int gap_cmd_adv_set      (cc2540_t        *dev,
                          gap_adv_t        adv_type,
                          uint8_t          data_len,
                          const uint8_t    data[]);
int gap_cmd_disc_end     (cc2540_t        *dev);
int gap_cmd_link_set     (cc2540_t        *dev,
                          bool             high_duty,
                          bool             white_list,
                          gap_addr_t       addr_type,
                          const uint8_t    addr[BT_ADDR_LEN]);
int gap_cmd_link_end     (cc2540_t        *dev,
                          uint16_t         handle,
                          gap_reason_t     reason);
int gap_cmd_auth         (cc2540_t        *dev,
                          uint16_t         handle,
                          gap_io_t         sec_io,
                          bool             sec_oob,
                          uint8_t          sec_oob_init[BT_LTK_LEN],
                          gap_auth_t       sec_auth,
                          uint8_t          sec_max_key_size,
                          gap_key_t        sec_key,
                          bool             pair_enable,
                          gap_io_t         pair_io,
                          bool             pair_oob,
                          gap_auth_t       pair_auth,
                          uint8_t          pair_max_key_size,
                          gap_key_t        pair_key);
int gap_cmd_pass_update  (cc2540_t        *dev,
                          uint16_t         handle,
                          const char       pass[GAP_PASS_STR_LEN]);
int gap_cmd_link_update  (cc2540_t        *dev,
                          uint16_t         handle,
                          uint16_t         min_interval,
                          uint16_t         max_interval,
                          uint16_t         latency,
                          uint16_t         timeout);
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
