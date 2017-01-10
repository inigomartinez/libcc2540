/*
 * Copyright (c) 2017 Iñigo Martínez <inigomartinez@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-3.0+
 */

#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cc2540.h>
#include <cc2540-cmd.h>
#include <cc2540-error.h>

#include "test-common.h"

int
main (int argc, char **argv) {
    int r = EXIT_SUCCESS;
    cc2540_t *dev;

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

    if ((r = test_adv (dev)) < 0)
        goto close_dev;

close_dev:
    cc2540_close (dev);

    return r;
}
