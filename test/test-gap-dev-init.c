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

#define MAX_TAGS          25
#define INIT_SIGN_COUNTER 1

int
main (int argc, char **argv) {
    int r = EXIT_SUCCESS;
    cc2540_t *dev;
    gap_evt_dev_init_done_t evt;

    if (argc < 2) {
        fprintf (stderr, "use: %s /dev/ttyUSB0\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!(dev = cc2540_open (argv[1]))) {
        fprintf (stderr, "Error in cc2540_open (%s): %s\n", argv[1], strerror (errno));
        return EXIT_FAILURE;
    }

    if ((r = gap_cmd_dev_init (dev,
                               GAP_PROFILE_OBSERVER,
                               MAX_TAGS,
                               (uint8_t[BT_IRK_LEN]) {0},
                               (uint8_t[BT_CSRK_LEN]) {0},
                               INIT_SIGN_COUNTER)) < 0) {
        fprintf (stderr, "Error in gap_cmd_dev_init: %s\n", strerror (-r));
        goto close_dev;
    }

    if ((r = gap_evt_dev_init_done (dev, &evt)) < 0) {
        fprintf (stderr, "Error in gap_evt_dev_init_done: %s\n", strerror (-r));
        goto close_dev;
    }

close_dev:
    cc2540_close (dev);

    return r;
}
