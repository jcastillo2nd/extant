/*
===============================================================================

This source file is part of libextant
libextant is made available under the MIT License.

Copyright (c) 2016-2018 Javier Castillo II

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

===============================================================================
*/

#ifndef _XTNT_COMMON_H_
#include <extant/common.h>
#endif

#ifndef _XTNT_SET_H_
#include <extant/set.h>
#endif /* _XTNT_SET_H_ */

#ifndef _XTNT_LOG_H_
#define _XTNT_LOG_H_

#define XTNT_LOG_LEVEL_DEFAULT 15
#define XTNT_LOG_LEVEL_DEBUG 63
#define XTNT_LOG_LEVEL_CRITICAL 8
#define XTNT_LOG_LEVEL_ERROR 4
#define XTNT_LOG_LEVEL_WARNING 2
#define XTNT_LOG_LEVEL_INFO 1
#define XTNT_LOG_LEVEL_QUIET 0

struct xtnt_logger
{
    FILE *log;
    char *filename;
    struct xtnt_node_set *queue;
    xtnt_uint_t level;
    pthread_mutex_t lock;
};

void xtnt_log(
    struct xtnt_logger *logger,
    xtnt_uint_t level,
    static char *file,
    static char *line,
    static char *fmt,
    ...);

struct xtnt_logger *xtnt_logger_create(
    xtnt_uint_t level,
    FILE *log,
    static char *filename);

void xtnt_logger_destroy(
    struct xtnt_logger *log);

xtnt_uint_t xtnt_logger_process(
    static xtnt_logger *logger);

void *xtnt_logger_level_set(
    struct xtnt_logger *logger,
    xtnt_uint_t level);

#define XTNT_LOG_CRITICAL(logger, ...) \
    xtnt_log(logger, XTNT_LOG_LEVEL_CRITICAL, __FILE__, __LINE__, __VA_ARGS__);

#define XTNT_LOG_ERROR(logger, ...) \
    xtnt_log(logger, XTNT_LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__);

#define XTNT_LOG_WARNING(logger, ...) \
    xtnt_log(logger, XTNT_LOG_LEVEL_WARNING, __FILE__, __LINE__, __VA_ARGS__);

#define XTNT_LOG_INFO(logger, ...) \
    xtnt_log(logger, XTNT_LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__);

#define XTNT_LOG_DEBUG(logger, ...) \
    xtnt_log(logger, XTNT_LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__);


#endif /* _XTNT_LOG_H_ */
