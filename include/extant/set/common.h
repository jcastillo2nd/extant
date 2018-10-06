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

#ifndef _XTNT_ERROR_H_
#include <extant/error.h>
#endif /* _XTNT_ERROR_H_ */

#define XTNT_NODE_HEAD 0
#define XTNT_NODE_LEFT 0
#define XTNT_NODE_MIDDLE 1
#define XTNT_NODE_CENTER 1
#define XTNT_NODE_PARENT 1
#define XTNT_NODE_TAIL 2
#define XTNT_NODE_RIGHT 2

/**
 * @struct xtnt_node
 * Operand of xtnt_node_set operations.
 * Attributes marked public can be accessed by any thread. All other attributes require a lock.
 */
struct xtnt_node {
/**
 * @public
 * Pointer to any value the node holds. 
 * @attention This property should be treated as immutable.
 */
    void *value;
/**
 * @private
 * Array of pointers to relative nodes.
 * @warning The pointers are managed by the set operations and may or may not
 * have valid values.
 */
    struct xtnt_node *link[3];
/** 
 * @private
 * An identifier for the node
 */
    xtnt_uint_t key;
/**
 * @private
 * The state of the node after operations
 */
    xtnt_uint_t state;
/**
 * @public
 * An attribute of the node.
 * @attention This property should be treated as immutable.
 * @note This may be used in place of a namespace, or for qualifying the node
 * as a particular type when referencing `xtnt_node::value`.
 */
    xtnt_uint_t quirk;
/**
 * @public
 * Lock used to operate on any members
 */
    pthread_mutex_t lock;
};

struct xtnt_node_set {
    struct xtnt_node *link[3];
    xtnt_uint_t count;
    xtnt_uint_t state;
    pthread_mutex_t lock;
};

xtnt_status_t
xtnt_node_initialize(
    struct xtnt_node *node,
    xtnt_uint_t key,
    xtnt_uint_t quirk,
    void *value);

xtnt_status_t
xtnt_node_set_copy(
    struct xtnt_node_set *src,
    struct xtnt_node_set *dst);

xtnt_status_t
xtnt_node_set_initialize(
    struct xtnt_node_set *set);

xtnt_status_t
xtnt_node_set_uninitialize(
    struct xtnt_node_set *set);

xtnt_status_t
xtnt_node_uninitialize(
    struct xtnt_node *node);

#endif /* ifndef _XTNT_SET_COMMON_H_ */
