/*
 * Copyright (c) 2017 Iñigo Martínez <inigomartinez@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-3.0+
 */

#ifndef __TEST_COMMON_H__
#define __TEST_COMMON_H__

#include <stdint.h>

#include <cc2540.h>
#include <cc2540-cmd.h>

int test_get_check    (cc2540_t  *dev,
                       hci_evt_t *evt,
                       uint16_t   code,
                       uint8_t    status);

int test_dev_init     (cc2540_t  *dev);
int test_dev_disc     (cc2540_t  *dev);
int test_adv          (cc2540_t  *dev);

#endif /* __TEST_COMMON_H__ */
