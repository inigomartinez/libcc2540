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

int test_get_check (cc2540_t      *dev,
                    hci_evt_t     *evt,
                    uint16_t       code,
                    uint8_t        status);

int test_dev_init  (cc2540_t      *dev,
                    gap_profile_t  profile);
int test_dev_disc  (cc2540_t      *dev);
int test_adv       (cc2540_t      *dev);
int test_link_set  (cc2540_t      *dev,
                    gap_addr_t     addr_type,
                    const uint8_t  addr[BT_ADDR_LEN],
                    hci_evt_t     *evt);
int test_link_end  (cc2540_t      *dev,
                    uint16_t       handle);

#endif /* __TEST_COMMON_H__ */
