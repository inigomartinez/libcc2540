/*
 * Copyright (c) 2016 Iñigo Martínez <inigomartinez@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-3.0+
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cc2540.h>

int
main (int argc, char **argv) {
    cc2540_t *dev;

    if (argc < 2) {
        fprintf (stderr, "use: %s /dev/ttyUSB0\n", argv[0]);
        return -1;
    }

    if (!(dev = cc2540_open (argv[1]))) {
        fprintf (stderr, "Could not open (%s): %s\n", argv[1], strerror (errno));
        return EXIT_FAILURE;
    }

    cc2540_close (dev);

    return EXIT_SUCCESS;
}
