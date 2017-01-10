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

#include "test-common.h"

#define MAX_TAGS          25
#define INIT_SIGN_COUNTER 1

#define MODE              GAP_SCAN_ALL
#define ACTIVE_SCAN       false
#define WHITE_LIST        false

int
test_dev_init (cc2540_t *dev) {
    int r = 0;
    hci_evt_t evt;

    if ((r = gap_cmd_dev_init (dev,
                               GAP_PROFILE_OBSERVER | GAP_PROFILE_BROADCASTER,
                               MAX_TAGS,
                               (uint8_t[BT_IRK_LEN]) {0},
                               (uint8_t[BT_CSRK_LEN]) {0},
                               INIT_SIGN_COUNTER)) < 0) {
        fprintf (stderr, "Error in gap_cmd_dev_init: %s\n", strerror (-r));
        return r;
    }

    if ((r = hci_evt (dev, &evt) < 0)) {
        fprintf (stderr, "Error in hci_evt: %s\n", strerror (-r));
        return r;
    }

    assert (HCI_EVT_IS (evt, GAP_EVT_DEV_INIT_DONE));

    return r;
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
