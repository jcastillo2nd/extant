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

#include <extant/error.h>

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


struct xtnt_node_set_state {
    xtnt_uint_t size;
    xtnt_uint_t count;
    xtnt_uint_t state;
}

struct xtnt_node_set_fn; // Declare

struct xtnt_node_set {
    const struct xtnt_node_set_fn const *fn;
    struct xtnt_node *link[3];
    xtnt_uint_t size;
    xtnt_uint_t count;
    xtnt_uint_t state;
    xtnt_node_set_fn fn;
    pthread_mutex_t lock;
}

/* See https://stackoverflow.com/questions/17621544/dynamic-method-dispatching-in-c/17622474#17622474 */

struct xtnt_node_set_fn {
/* Accessors */
    xtnt_status_t (*index)(struct xtnt_node_set *set, xtnt_int_t index, struct xtnt_node **found);
    xtnt_status_t (*search)(xtnt_uint_t, struct xtnt_node **found);
    xtnt_status_t (*search_fn)(void *fn, struct xtnt_node **found);
    xtnt_status_t (*first)(struct xtnt_node **first);
    xtnt_status_t (*last)(struct xtnt_node **last);
    xtnt_status_t (*peek)(struct xtnt_node **peek);
    xtnt_status_t (*root)(struct xtnt_node **root);
    xtnt_status_t (*state)(struct xtnt_node_set_state **state);
/* Modifiers */
    xtnt_status_t (*insert)(struct xtnt_node *node);
    xtnt_status_t (*insert_at)(xtnt_uint_t index, struct xtnt_node *node, struct xtnt_node **replaced);
    xtnt_status_t (*push)(struct xtnt_node *node);

    xtnt_status_t (*remove)(struct xtnt_node *node);
    xtnt_status_t (*remove_at)(xtnt_uint_t index, struct xtnt_node **removed);
    xtnt_status_t (*pop)(struct xtnt_node *node);

    xtnt_status_t (*sort)();
    xtnt_status_t (*sort_reverse)();
    xtnt_status_t (*sort_fn)(void *fn);

    xtnt_status_t (*grow)();
    xtnt_status_t (*grow_eval)();
    xtnt_status_t (*shrink)();
    xtnt_status_t (*shrink_eval)();
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
