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

static const char *hci_type_str[] = {
    [HCI_TYPE_CMD]        = "Command",
    [HCI_TYPE_ASYNC_DATA] = "Async data",
    [HCI_TYPE_SYNC_DATA]  = "Sync data",
    [HCI_TYPE_EVENT]      = "Event"
};

static const char *adv_type_str[] = {
    [GAP_ADV_SCAN_RSP]  = "Scan response",
    [GAP_ADV_RAW]       = "Raw advertisement"
};

static const char *evt_type_str[] = {
    [GAP_EVT_CONN_UNDIR]    = "Connectable undirected advertisement",
    [GAP_EVT_CONN_DIR]      = "Connectable directed advertisement",
    [GAP_EVT_DISC_UNDIR]    = "Discoverable undirected advertisement",
    [GAP_EVT_NO_CONN_UNDIR] = "Non-connectable undirected advertisement",
    [GAP_EVT_SCAN_RESP]     = "Scan-Response"
};

static const char *addr_type_str[] = {
    [GAP_ADDR_PUBLIC]          = "Public",
    [GAP_ADDR_STATIC]          = "Static",
    [GAP_ADDR_PRIV_NON_RESOLV] = "Private non resolvable",
    [GAP_ADDR_PRIV_RESOLV]     = "Private resolvable",
};

static const char *clock_str[] = {
    [GAP_CLOCK_500] = "500 ppm",
    [GAP_CLOCK_250] = "250 ppm",
    [GAP_CLOCK_150] = "150 ppm",
    [GAP_CLOCK_100] = "100 ppm",
    [GAP_CLOCK_75]  = "75 ppm",
    [GAP_CLOCK_50]  = "50 ppm",
    [GAP_CLOCK_30]  = "30 ppm",
    [GAP_CLOCK_20]  = "20 ppm"
};

static const char *reason_str[] = {
    [GAP_REASON_AUTH_FAIL]                  = "Authentication failure",
    [GAP_REASON_TIMEOUT]                    = "Timeout",
    [GAP_REASON_PEER]                       = "Peer disconnected",
    [GAP_REASON_PEER_LOW_RES]               = "Peer disconnected on low resources",
    [GAP_REASON_PEER_POWER_OFF]             = "Peer disconnected on power off",
    [GAP_REASON_HOST]                       = "Host disconnected",
    [GAP_REASON_PEER_NO_FEATURE]            = "Peer unsupported feature",
    [GAP_REASON_CTRL_PACKET_TIMEOUT]        = "Control packet timeout",
    [GAP_REASON_CTRL_PACKET_INSTANT_PASSED] = "Control packet instant passed",
    [GAP_REASON_UNIT_KEY_NOT_SUPPORTED]     = "Unit key not supported",
    [GAP_REASON_BAD_CONN_INTERVAL]          = "Bad connection interval",
    [GAP_REASON_MIC_FAIL]                   = "MIC failure"
};

static const char *auth_str[] = {
    [GAP_AUTH_BONDING]   = "Bonding",
    [GAP_AUTH_MITM_PROT] = "Man-In-The-Middle protection",
    [GAP_AUTH_LE_SEC]    = "LE Secure connection pairing"
};

CC2540_EXPORT void
print_hci_evt_info (const hci_evt_info_t *evt) {
    printf ("HCI\n");
    printf ("type: %s\n", hci_type_str[evt->type]);
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
print_gap_evt_adv_set_done (const gap_evt_adv_set_done_t *evt) {
    printf ("GAP_AdvertDataUpdateDone\n");
    printf ("status: 0x%02x\n", evt->status);
    printf ("adv_type: %s\n", adv_type_str[evt->adv_type]);
}

CC2540_EXPORT void
print_gap_evt_dev_disc (const gap_evt_dev_disc_t *evt) {
    printf ("GAP_DeviceDiscovery\n");
    printf ("status: 0x%02x\n", evt->status);
    printf ("num_devs: %u\n", evt->num_devs);
    for (uint8_t i = 0; i < evt->num_devs; i++) {
        printf ("evt_type: %s\n", evt_type_str[evt->dev[i].evt_type]);
        printf ("addr_type: %s\n", addr_type_str[evt->dev[i].addr_type]);
        printf ("addr: %02x", evt->dev[i].addr[0]);
        for (uint8_t n = 1; n < BT_ADDR_LEN; n++)
            printf (":%02x", evt->dev[i].addr[n]);
        printf ("\n");
    }
}

CC2540_EXPORT void
print_gap_evt_disc_set_done (const gap_evt_disc_set_done_t *evt) {
    printf ("GAP_MakeDiscoverableDone\n");
    printf ("status: 0x%02x\n", evt->status);
    printf ("interval: %u\n", evt->interval);
}

CC2540_EXPORT void
print_gap_evt_link_set (const gap_evt_link_set_t *evt) {
    printf ("GAP_LinkEstablished\n");
    printf ("status: 0x%02x\n", evt->status);
    printf ("addr_type: %s\n", addr_type_str[evt->addr_type]);
    printf ("addr: %02x", evt->addr[0]);
    for (uint8_t n = 1; n < BT_ADDR_LEN; n++)
        printf (":%02x", evt->addr[n]);
    printf ("\n");
    printf ("handle: %04x\n", evt->handle);
    printf ("role: ");
    if (evt->role & GAP_PROFILE_BROADCASTER)
        printf ("|GAP_PROFILE_BROADCASTER|");
    if (evt->role & GAP_PROFILE_OBSERVER)
        printf ("|GAP_PROFILE_OBSERVER|");
    if (evt->role & GAP_PROFILE_PERIPHERAL)
        printf ("|GAP_PROFILE_PERIPHERAL|");
    if (evt->role & GAP_PROFILE_CENTRAL)
        printf ("|GAP_PROFILE_CENTRAL|");
    printf ("\n");
    printf ("interval: %f msec\n", GAP_INTERVAL_TO_MSEC (evt->interval));
    printf ("latency: %04x\n", evt->latency);
    printf ("timeout: %u msec\n", GAP_TIMEOUT_TO_MSEC (evt->timeout));
    printf ("clock_accuracy: %s\n", clock_str[evt->clock_accuracy]);
}

CC2540_EXPORT void
print_gap_evt_link_end (const gap_evt_link_end_t *evt) {
    printf ("GAP_LinkTerminated\n");
    printf ("status: 0x%02x\n", evt->status);
    printf ("handle: %04x\n", evt->handle);
    printf ("reason: %s\n", reason_str[evt->reason]);
}

CC2540_EXPORT void
print_gap_evt_link_update (const gap_evt_link_update_t *evt) {
    printf ("GAP_LinkParamUpdate\n");
    printf ("status: 0x%02x\n", evt->status);
    printf ("handle: %04x\n", evt->handle);
    printf ("interval: %f msec\n", GAP_INTERVAL_TO_MSEC (evt->interval));
    printf ("latency: %04x\n", evt->latency);
    printf ("timeout: %u msec\n", GAP_TIMEOUT_TO_MSEC (evt->timeout));
}

CC2540_EXPORT void
print_gap_evt_auth (const gap_evt_auth_t *evt) {
    printf ("GAP_AuthenticationComplete\n");
    printf ("status: 0x%02x\n", evt->status);
    printf ("handle: 0x%04x\n", evt->handle);
    printf ("auth: %s\n", auth_str[evt->auth]);
    printf ("sec_info: %s\n", (evt->sec_info_status ? "yes" : "no"));
    if (evt->sec_info_status) {
        printf ("ltk_size: %u\n", evt->sec_info.ltk_size);
        printf ("ltk: %02x", evt->sec_info.ltk[0]);
        for (uint8_t n = 1; n < BT_LTK_LEN; n++)
            printf (":%02x", evt->sec_info.ltk[n]);
        printf ("\n");
        printf ("div: 0x%04x\n", evt->sec_info.div);
        printf ("rand: %02x", evt->sec_info.rand[0]);
        for (uint8_t n = 1; n < BT_LTK_RAND_LEN; n++)
            printf (":%02x", evt->sec_info.rand[n]);
        printf ("\n");
    }
    printf ("dev_sec_info: %s\n", (evt->dev_sec_info_status ? "yes" : "no"));
    if (evt->dev_sec_info_status) {
        printf ("ltk_size: %u\n", evt->dev_sec_info.ltk_size);
        printf ("ltk: %02x", evt->dev_sec_info.ltk[0]);
        for (uint8_t n = 1; n < BT_LTK_LEN; n++)
            printf (":%02x", evt->dev_sec_info.ltk[n]);
        printf ("\n");
        printf ("div: 0x%04x\n", evt->dev_sec_info.div);
        printf ("rand: %02x", evt->dev_sec_info.rand[0]);
        for (uint8_t n = 1; n < BT_LTK_RAND_LEN; n++)
            printf (":%02x", evt->dev_sec_info.rand[n]);
        printf ("\n");
    }
    printf ("ident_info: %s\n", (evt->ident_info_status ? "yes" : "no"));
    if (evt->ident_info_status) {
        printf ("irk: %02x\n", evt->ident_info.irk[0]);
        for (uint8_t n = 1; n < BT_IRK_LEN; n++)
            printf (":%02x", evt->ident_info.irk[n]);
        printf ("\n");
        printf ("addr: %02x\n", evt->ident_info.addr[0]);
        for (uint8_t n = 1; n < BT_ADDR_LEN; n++)
            printf (":%02x", evt->ident_info.addr[n]);
        printf ("\n");
    }
    printf ("sign_info: %s\n", (evt->sign_info_status ? "yes" : "no"));
    if (evt->sign_info_status) {
        printf ("irk: %02x\n", evt->sign_info.irk[0]);
        for (uint8_t n = 1; n < BT_IRK_LEN; n++)
            printf (":%02x", evt->sign_info.irk[n]);
        printf ("\n");
        printf ("sign_counter: 0x%04x\n", evt->sign_info.sign_counter);
    }
}

CC2540_EXPORT void
print_gap_evt_dev_info (const gap_evt_dev_info_t *evt) {
    printf ("GAP_DeviceInformation\n");
    printf ("status: 0x%02x\n", evt->status);
    printf ("evt_type: %s\n", evt_type_str[evt->evt_type]);
    printf ("addr_type: %s\n", addr_type_str[evt->addr_type]);
    printf ("addr: %02x", evt->addr[0]);
    for (uint8_t n = 1; n < BT_ADDR_LEN; n++)
        printf (":%02x", evt->addr[n]);
    printf ("\n");
    printf ("rssi: %d\n", evt->rssi);
    printf ("data_len: %u\n", evt->data_len);
    if (evt->data_len) {
        printf ("data: %02x", evt->data[0]);
        for (uint8_t n = 1; n < evt->data_len; ++n)
            printf (":%02x", evt->data[n]);
        printf ("\n");
    }
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
