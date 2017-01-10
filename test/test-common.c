/*
 * Copyright (c) 2017 Iñigo Martínez <inigomartinez@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-3.0+
 */

#include <sys/time.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <cc2540.h>
#include <cc2540-cmd.h>
#include <cc2540-error.h>

#include "test-common.h"

#define MAX_TAGS          25
#define INIT_SIGN_COUNTER 1

#define MODE              GAP_SCAN_ALL
#define ACTIVE_SCAN       false
#define WHITE_LIST        false

#define IBEACON_UUID_LEN 16

struct {
    struct {
        uint8_t length;
        uint8_t type;
        uint8_t data;
    } adv_flags;
    struct {
        uint8_t length;
        uint8_t type;
    } adv_header;
    uint16_t com_id;
    uint8_t  type;
    uint8_t  length;
    uint8_t  uuid[IBEACON_UUID_LEN];
    uint16_t major;
    uint16_t minor;
    uint8_t  rssi;
} __attribute__((packed)) adv = {
    .adv_flags = {
        .length = 0x02,
        .type = 0x01,
        .data = 0x06
    },
    .adv_header = {
        .length = 0x1A,
        .type = 0xFF,
    },
    .com_id = 0x4C00,
    .type = 0x02,
    .length = 0x15,
    .uuid = {0xB9, 0x40, 0x7F, 0x30, 0xF5, 0xF8, 0x46, 0x6E,
             0xAF, 0xF9, 0x25, 0x55, 0x6B, 0x57, 0xFE, 0x6D},
    .major = 0x0049,
    .minor = 0x000A,
    .rssi = 0xC5
};

int
test_get_check (cc2540_t  *dev,
                hci_evt_t *evt,
                uint16_t   code,
                uint8_t    status) {
    int r = 0;

    if ((r = hci_evt (dev, evt)) < 0) {
        fprintf (stderr, "Error in hci_evt: %s\n", strerror (-r));
        return r;
    }

    if (!HCI_EVT_IS (*evt, code)) {
        fprintf (stderr, "Error bad event code: %04x\n", evt->evt_code);
        return -1;
    }

    if (evt->evt.status != status) {
        fprintf (stderr, "Error bad status (%02x): %s\n", evt->evt.status,
                                                          hci_strerror (evt->evt.status));
        return -1;
    }

    return r;
}

int
test_dev_init (cc2540_t      *dev,
               gap_profile_t  profile) {
    int r;
    hci_evt_t evt;

    if ((r = gap_cmd_dev_init (dev,
                               profile,
                               MAX_TAGS,
                               (uint8_t[BT_IRK_LEN]) {0},
                               (uint8_t[BT_CSRK_LEN]) {0},
                               INIT_SIGN_COUNTER)) < 0) {
        fprintf (stderr, "Error in gap_cmd_dev_init: %s\n", strerror (-r));
        return r;
    }

    return test_get_check (dev, &evt, GAP_EVT_DEV_INIT_DONE, 0);
}

int
test_dev_disc (cc2540_t *dev) {
    int r = 0;
    uint16_t scan_time;

    if ((r = gap_cmd_param_get (dev, TGAP_GEN_DISC_SCAN, &scan_time)) < 0) {
        fprintf (stderr, "Error in gap_cmd_param_get: %s\n", strerror (-r));
        return r;
    }

    if ((r = gap_cmd_dev_disc (dev, MODE, ACTIVE_SCAN, WHITE_LIST)) < 0) {
        fprintf (stderr, "Error in gap_cmd_dev_disc: %s\n ", strerror (-r));
        return r;
    }
    cc2540_set_timeout (dev, (scan_time + 1000));

    return r;
}

int
test_adv (cc2540_t *dev) {
    int r = 0;
    hci_evt_t evt;

    if ((r = gap_cmd_adv_set (dev, GAP_ADV_RAW, sizeof (adv), (uint8_t *) &adv)) < 0) {
        fprintf (stderr, "Error in gap_cmd_adv_set: %s\n", strerror (-r));
        return r;
    }

    return test_get_check (dev, &evt, GAP_EVT_ADV_SET_DONE, 0);
}
