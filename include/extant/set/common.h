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

#ifndef _XTNT_SET_COMMON_H_
#define _XTNT_SET_COMMON_H_

#ifndef _XTNT_COMMON_H_
#include <extant/common.h>
#endif

#define XTNT_NODE_HEAD 0
#define XTNT_NODE_LEFT 0
#define XTNT_NODE_MIDDLE 1
#define XTNT_NODE_CENTER 1
#define XTNT_NODE_PARENT 1
#define XTNT_NODE_TAIL 2
#define XTNT_NODE_RIGHT 2

struct xtnt_node {
    void *value;
    struct xtnt_node *link[3];
    xtnt_uint_t key;
    xtnt_uint_t mode;
    xtnt_uint_t state;
    xtnt_uint_t dir;
    pthread_mutex_t lock;
};

struct xtnt_node_set {
    struct xtnt_node *link[3];
    xtnt_uint_t count;
    xtnt_uint_t state;
    pthread_mutex_t lock;
};

xtnt_int_t xtnt_node_initialize(
        struct xtnt_node *node,
        xtnt_uint_t key,
        void *value);

xtnt_int_t xtnt_node_set_initialize(
        struct xtnt_node_set *set);

xtnt_int_t xtnt_node_uninitialize(
        struct xtnt_node *node);

xtnt_int_t xtnt_node_set_uninitialize(
        struct xtnt_node_set *set);

#endif /* ifndef _XTNT_SET_COMMON_H_ */
