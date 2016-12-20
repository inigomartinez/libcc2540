/*
 * Copyright (c) 2016 Iñigo Martínez <inigomartinez@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-3.0+
 */

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "cc2540-private.h"
#include "cc2540.h"

static int set_interface_attribs (cc2540_t *dev,
                                  speed_t   speed);

CC2540_EXPORT cc2540_t *
cc2540_open (const char *pathname) {
    cc2540_t *dev;

    if (!pathname) {
        errno = EINVAL;
        return NULL;
    }

    if (!(dev = malloc (sizeof (cc2540_t))))
        return NULL;

    if ((dev->dev = open (pathname, O_RDWR | O_NOCTTY | O_SYNC)) == -1) {
        free (dev);
        return NULL;
    }

    if (set_interface_attribs (dev, _CC2540_DEV_SPEED) == -1) {
        close (dev->dev);
        free (dev);
        return NULL;
    }

    dev->pfd[0].fd = dev->dev;
    dev->pfd[0].events = POLLIN;

    dev->timeout = _CC2540_DEV_TIMEOUT;

    return dev;
}

CC2540_EXPORT void
cc2540_close (cc2540_t *dev) {
    close (dev->dev);
    free (dev);
}

CC2540_EXPORT ssize_t
cc2540_read (cc2540_t *dev,
             void     *buf,
             size_t    len) {
    if (poll (dev->pfd, 1, dev->timeout) <= 0)
        return -1;

    return read (dev->dev, buf, len);
}

CC2540_EXPORT ssize_t
cc2540_write (cc2540_t *dev,
              void     *buf,
              size_t    len) {
    return write (dev->dev, buf, len);
}

CC2540_EXPORT void
cc2540_get_timeout (cc2540_t *dev,
                    int      *timeout) {
    *timeout = dev->timeout;
}

CC2540_EXPORT void
cc2540_set_timeout (cc2540_t *dev,
                    int       timeout) {
    dev->timeout = timeout;
}

static int
set_interface_attribs (cc2540_t *dev,
                       speed_t   speed) {
    struct termios tty;

    if (tcgetattr (dev->dev, &tty) == -1)
        return -1;

    if (cfsetspeed (&tty, speed) == -1)
        return -1;

    // disable break processing
    tty.c_iflag &= ~IGNBRK;
    // shut off xon/xoff ctrl
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    // no CR <-> NL translation
    tty.c_iflag &= ~(ICRNL | INLCR | ISTRIP);

    // no remapping, no delays
    tty.c_oflag = 0;

    // 8-bit chars
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    // ignore modem controls, enable reading
    tty.c_cflag |= (CLOCAL | CREAD);
    // shut off parity
    tty.c_cflag &= ~(PARENB | PARODD);
    tty.c_cflag &= ~CSTOPB;
    // no flow control
    tty.c_cflag &= ~CRTSCTS;

    // no signaling chars, no echo, no canonical processing
    tty.c_lflag = 0;

    // read doesn't block
    tty.c_cc[VMIN] = 1;
    // 0.5 seconds read timeout
    tty.c_cc[VTIME] = 0;

    // flush port
    if (tcflush (dev->dev, TCIOFLUSH) == -1)
        return -1;

    return tcsetattr (dev->dev, TCSANOW, &tty);
}
