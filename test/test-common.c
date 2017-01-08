/*
 * Copyright (c) 2017 Iñigo Martínez <inigomartinez@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-3.0+
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <cc2540.h>
#include <cc2540-cmd.h>

#include "test-common.h"

#define MAX_TAGS          25
#define INIT_SIGN_COUNTER 1

int
test_dev_init (cc2540_t *dev) {
    int r = 0;
    hci_evt_t evt;

    if ((r = gap_cmd_dev_init (dev,
                               GAP_PROFILE_OBSERVER,
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
