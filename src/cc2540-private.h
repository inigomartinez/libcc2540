/*
 * Copyright (c) 2016 Iñigo Martínez <inigomartinez@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-3.0+
 */

#ifndef __CC2540_PRIVATE_H__
#define __CC2540_PRIVATE_H__

#include <sys/time.h>
#include <poll.h>
#include <termios.h>

#include "cc2540.h"

CC2540_BEGIN_DECLS

#define CC2540_EXPORT __attribute__ ((visibility("default")))

#define _CC2540_DEV_SPEED   B115200
#define _CC2540_DEV_TIMEOUT 500

struct _cc2540_t {
    int           dev;
    struct pollfd pfd[1];
    int           timeout;
};

CC2540_END_DECLS

#endif /* __CC2540_PRIVATE_H__ */
