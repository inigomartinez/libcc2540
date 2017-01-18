/*
 * Copyright (c) 2017 Iñigo Martínez <inigomartinez@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-3.0+
 */

#include <assert.h>
#include <errno.h>
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
    hci_evt_t disc_evt, link_evt, evt;
    gap_evt_dev_disc_t *disc;
    gap_evt_link_set_t *link;

    if (argc < 2) {
        fprintf (stderr, "use: %s /dev/ttyUSB0\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!(dev = cc2540_open (argv[1]))) {
        fprintf (stderr, "Error in cc2540_open (%s): %s\n", argv[1], strerror (errno));
        return EXIT_FAILURE;
    }

    if ((r = test_dev_init (dev, GAP_PROFILE_CENTRAL)) < 0)
        goto close_dev;

    if ((r = test_dev_disc (dev)) < 0)
        goto close_dev;

    do {
        if ((r = hci_evt (dev, &disc_evt)) < 0) {
            fprintf (stderr, "Error in hci_evt: %s\n", strerror (-r));
            goto close_dev;
        }

        if (disc_evt.evt.status) {
            fprintf (stderr, "Error in hci_evt: %s\n", hci_strerror (disc_evt.evt.status));
            r = EXIT_FAILURE;
            goto close_dev;
        }

        assert (HCI_EVT_IS (disc_evt, GAP_EVT_DEV_INFO) ||
                HCI_EVT_IS (disc_evt, GAP_EVT_DEV_DISC));
    } while (!HCI_EVT_IS (disc_evt, GAP_EVT_DEV_DISC));

    disc = GAP_EVT_DEV_DISC_T (&(disc_evt.evt));
    if (!disc->num_devs)
        goto close_dev;

    if ((r = test_link_set (dev,
                            disc->dev[0].addr_type,
                            disc->dev[0].addr,
                            &link_evt)) < 0)
        goto close_dev;

    link = GAP_EVT_LINK_SET_T (&(link_evt.evt));

    test_get_check (dev, &evt, GAP_EVT_LINK_UPDATE, 0);

    if ((r = gap_cmd_auth (dev, link->handle,
                           GAP_IO_NO_IO,
                           true,
                           (uint8_t[BT_LTK_LEN]) {0},
                           GAP_AUTH_BONDING,
                           7,
                           GAP_KEY_SLAVE_LTK | GAP_KEY_MASTER_LTK,
                           true,
                           GAP_IO_NO_IO,
                           false,
                           GAP_AUTH_LE_SEC,
                           7,
                           GAP_KEY_SLAVE_LTK | GAP_KEY_MASTER_LTK)) < 0) {
            fprintf (stderr, "Error in gap_cmd_auth: %s\n", strerror (-r));
            goto close_link;
    }

    r = test_get_check (dev, &evt, GAP_EVT_AUTH, 0);
close_link:
    test_link_end (dev, link->handle);
close_dev:
    cc2540_close (dev);

    return r;
}
