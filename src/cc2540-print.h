/*
 * Copyright (c) 2017 Iñigo Martínez <inigomartinez@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-3.0+
 */

#ifndef __CC2540_PRINT_H__
#define __CC2540_PRINT_H__

#include "cc2540.h"
#include "cc2540-cmd.h"

CC2540_BEGIN_DECLS

void print_hci_evt_info          (const hci_evt_info_t          *evt);
void print_gap_evt_dev_init_done (const gap_evt_dev_init_done_t *evt);
void print_gap_evt_dev_disc      (const gap_evt_dev_disc_t      *evt);
void print_gap_evt_dev_info      (const gap_evt_dev_info_t      *evt);
void print_gap_evt_cmd_status    (const gap_evt_cmd_status_t    *evt);

CC2540_END_DECLS

#endif /* __CC2540_PRINT_H__ */
