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
main (int argc, char **argv) {
    int r = EXIT_SUCCESS;
    cc2540_t *dev;
    hci_evt_t evt;

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

    if ((r = gap_cmd_adv_set (dev, GAP_ADV_RAW, sizeof (adv), (uint8_t *) &adv)) < 0) {
        fprintf (stderr, "Error in gap_cmd_adv_set: %s\n", strerror (-r));
        goto close_dev;
    }

    if ((r = hci_evt (dev, &evt)) < 0) {
        fprintf (stderr, "Error in hci_evt: %s\n", strerror (-r));
        goto close_dev;
    }

    if (evt.evt.status) {
        fprintf (stderr, "Error in hci_evt: %s\n", hci_strerror (evt.evt.status));
        r = EXIT_FAILURE;
        goto close_dev;
    }

    assert (HCI_EVT_IS (evt, GAP_EVT_ADV_SET_DONE));
close_dev:
    cc2540_close (dev);

    return r;
}
