/*
 * Copyright (c) 2017 Iñigo Martínez <inigomartinez@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-3.0+
 */

#include <sys/time.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cc2540.h>
#include <cc2540-cmd.h>

#include "test-common.h"

#define MODE        GAP_SCAN_ALL
#define ACTIVE_SCAN false
#define WHITE_LIST  false

int
main (int argc, char **argv) {
    int r = EXIT_SUCCESS;
    cc2540_t *dev;
    hci_evt_t evt;
    uint16_t scan_time;

    if (argc < 2) {
        fprintf (stderr, "use: %s /dev/ttyUSB0\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!(dev = cc2540_open (argv[1]))) {
        fprintf (stderr, "Error in cc2540_open (%s): %s\n", argv[1], strerror (errno));
        return EXIT_FAILURE;
    }

    if ((r = test_dev_init (dev)) < 0)
        goto close_dev;

    if ((r = gap_cmd_param_get (dev, TGAP_GEN_DISC_SCAN, &scan_time)) < 0) {
        fprintf (stderr, "Error in gap_cmd_param_get: %s\n", strerror (-r));
        goto close_dev;
    }

    if ((r = gap_cmd_dev_disc (dev, MODE, ACTIVE_SCAN, WHITE_LIST)) < 0) {
        fprintf (stderr, "Error in gap_cmd_dev_disc: %s\n ", strerror (-r));
        goto close_dev;
    }
    cc2540_set_timeout (dev, (scan_time + 1000));

    do {
        if ((r = hci_evt (dev, &evt)) < 0) {
            fprintf (stderr, "Error in hci_evt: %s\n", strerror (-r));
            goto close_dev;
        }

        assert (HCI_EVT_IS (evt, GAP_EVT_DEV_INFO) ||
                HCI_EVT_IS (evt, GAP_EVT_DEV_DISC));
    } while (!HCI_EVT_IS (evt, GAP_EVT_DEV_DISC));
close_dev:
    cc2540_close (dev);

    return r;
}