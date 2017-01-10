/*
 * Copyright (c) 2017 Iñigo Martínez <inigomartinez@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-3.0+
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cc2540.h>
#include <cc2540-cmd.h>

#include "test-common.h"

int
main (int argc, char **argv) {
    int r = EXIT_SUCCESS;
    cc2540_t *dev;
    hci_evt_t evt;
    uint16_t adv_time;

    if (argc < 2) {
        fprintf (stderr, "use: %s /dev/ttyUSB0\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!(dev = cc2540_open (argv[1]))) {
        fprintf (stderr, "Error in cc2540_open (%s): %s\n", argv[1], strerror (errno));
        return EXIT_FAILURE;
    }

    if ((r = test_dev_init (dev, GAP_PROFILE_BROADCASTER)) < 0)
        goto close_dev;

    if ((r = gap_cmd_param_get (dev, TGAP_GEN_DISC_ADV_INT_MAX, &adv_time)) < 0) {
        fprintf (stderr, "Error in gap_cmd_param_get: %s\n", strerror (-r));
        return r;
    }

    if ((r = test_adv (dev)) < 0)
        goto close_dev;

    if ((r = gap_cmd_disc_set (dev,
                               GAP_EVT_DISC_UNDIR,
                               GAP_ADDR_PUBLIC,
                               (uint8_t[BT_ADDR_LEN]) {0},
                               GAP_CHANNEL_37 | GAP_CHANNEL_38 | GAP_CHANNEL_39,
                               GAP_FILTER_SCAN_ALL_CONN_ALL)) < 0) {
        fprintf (stderr, "Error in gap_cmd_disc_set: %s\n ", strerror (-r));
        goto close_dev;
    }
    cc2540_set_timeout (dev, (adv_time + 1000));

    if ((r = test_get_check (dev, &evt, GAP_EVT_DISC_SET_DONE, 0)) < 0)
        goto close_dev;

    if ((r = gap_cmd_disc_end (dev)) < 0) {
        fprintf (stderr, "Error in gap_cmd_disc_end: %s\n", strerror (-r));
        goto close_dev;
    }

    r = test_get_check (dev, &evt, GAP_EVT_DISC_END, 0);
close_dev:
    cc2540_close (dev);

    return r;
}
