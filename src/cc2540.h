/*
 * Copyright (c) 2016-2017 Iñigo Martínez <inigomartinez@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-3.0+
 */

#ifndef __CC2540_H__
#define __CC2540_H__

#include <sys/time.h>
#include <stdlib.h>
#include <termios.h>

#ifdef  __cplusplus
# define CC2540_BEGIN_DECLS  extern "C" {
# define CC2540_END_DECLS    }
#else
# define CC2540_BEGIN_DECLS
# define CC2540_END_DECLS
#endif

CC2540_BEGIN_DECLS

typedef struct _cc2540_t cc2540_t;

cc2540_t *cc2540_open        (const char *pathname);
void      cc2540_close       (cc2540_t   *dev);

ssize_t   cc2540_read        (cc2540_t   *dev,
                              void       *buf,
                              size_t      len);
ssize_t   cc2540_write       (cc2540_t   *dev,
                              const void *buf,
                              size_t      len);

void      cc2540_get_timeout (cc2540_t   *dev,
                              int        *timeout);
void      cc2540_set_timeout (cc2540_t   *dev,
                              int         timeout);

CC2540_END_DECLS

#endif /* __CC2540_H__ */
