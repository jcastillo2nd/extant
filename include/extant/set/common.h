/*
===============================================================================

This source file is part of libextant
libextant is made available under the MIT License.

Copyright (c) 2016-2020 Javier Castillo II

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
#include <extant/set/node.h>

struct xtnt_node_set_if; // Forward Declaration

struct xtnt_node_set {
    union {
        struct xtnt_node root;
    };
    xtnt_uint_t count;
    xtnt_uint_t size;
    const struct xtnt_node_set_if *fn;
    pthread_mutex_t lock;
};

/* See https://stackoverflow.com/questions/17621544/dynamic-method-dispatching-in-c/17622474#17622474 */

struct xtnt_node_set_if {
/* Accessors */
    xtnt_status_t (*index)(struct xtnt_node_set *set, xtnt_int_t index, struct xtnt_node **found);
    xtnt_status_t (*search)(struct xtnt_node_set *set, xtnt_uint_t key, struct xtnt_node **found);
    xtnt_status_t (*search_fn)(struct xtnt_node_set *set, void *fn, void *needle, struct xtnt_node **found);
    xtnt_status_t (*first)(struct xtnt_node_set *set, struct xtnt_node **first);
    xtnt_status_t (*last)(struct xtnt_node_set *set, struct xtnt_node **last);
    xtnt_status_t (*peek)(struct xtnt_node_set *set, struct xtnt_node **peek);
    xtnt_status_t (*root)(struct xtnt_node_set *set, struct xtnt_node **root);
    xtnt_status_t (*state)(struct xtnt_node_set *set, xtnt_uint_t **state);
/* Modifiers */
    xtnt_status_t (*insert)(struct xtnt_node_set *set, struct xtnt_node *node);
    xtnt_status_t (*insert_at)(struct xtnt_node_set *set, xtnt_uint_t index, struct xtnt_node *node, struct xtnt_node **replaced);
    xtnt_status_t (*push)(struct xtnt_node_set *set, struct xtnt_node *node);

    xtnt_status_t (*remove)(struct xtnt_node_set *set, struct xtnt_node *node);
    xtnt_status_t (*remove_at)(struct xtnt_node_set *set, xtnt_uint_t index, struct xtnt_node **removed);
    xtnt_status_t (*pop)(struct xtnt_node_set *set, struct xtnt_node **node);

    xtnt_status_t (*sort)(struct xtnt_node_set *set);
    xtnt_status_t (*sort_reverse)(struct xtnt_node_set *set);
    xtnt_status_t (*sort_fn)(struct xtnt_node_set *set, void *fn);

    xtnt_status_t (*grow)(struct xtnt_node_set *set);
    xtnt_status_t (*grow_eval)(struct xtnt_node_set *set, size_t eval);
    xtnt_status_t (*shrink)(struct xtnt_node_set *set);
    xtnt_status_t (*shrink_eval)(struct xtnt_node_set *set, size_t eval);
};

inline xtnt_status_t
xtnt_set_index(
    struct xtnt_node_set *set,
    xtnt_int_t index,
    struct xtnt_node **found);

inline xtnt_status_t
xtnt_set_search(
    struct xtnt_node_set *set,
    xtnt_uint_t key,
    struct xtnt_node **found);

inline xtnt_status_t
xtnt_set_search_fn(
    struct xtnt_node_set *set,
    void *fn,
    void *needle,
    struct xtnt_node **found);

inline xtnt_status_t
xtnt_set_first(
    struct xtnt_node_set *set,
    struct xtnt_node **first);

inline xtnt_status_t
xtnt_set_last(
    struct xtnt_node_set *set,
    struct xtnt_node **last);

inline xtnt_status_t
xtnt_set_peek(
    struct xtnt_node_set *set,
    struct xtnt_node **peek);

inline xtnt_status_t
xtnt_set_root(
    struct xtnt_node_set *set,
    struct xtnt_node **root);

inline xtnt_status_t
xtnt_set_state(
    struct xtnt_node_set *set,
    xtnt_uint_t **state);

inline xtnt_status_t
xtnt_set_insert(
    struct xtnt_node_set *set,
    struct xtnt_node *node);

inline xtnt_status_t
xtnt_set_insert_at(
    struct xtnt_node_set *set,
    xtnt_uint_t index,
    struct xtnt_node *node,
    struct xtnt_node **replaced);

inline xtnt_status_t
xtnt_set_push(
    struct xtnt_node_set *set,
    struct xtnt_node *node);

inline xtnt_status_t
xtnt_set_remove(
    struct xtnt_node_set *set,
    struct xtnt_node *node);

xtnt_status_t
inline xtnt_set_remove_at(
    struct xtnt_node_set *set,
    xtnt_uint_t index,
    struct xtnt_node **removed);

inline xtnt_status_t
xtnt_set_pop(
    struct xtnt_node_set *set,
    struct xtnt_node **node);

inline xtnt_status_t
xtnt_set_sort(
    struct xtnt_node_set *set);

inline xtnt_status_t
xtnt_set_sort_reverse(
    struct xtnt_node_set *set);

inline xtnt_status_t
xtnt_set_sort_fn(
    struct xtnt_node_set *set,
    void *fn);

inline xtnt_status_t
xtnt_set_grow(
    struct xtnt_node_set *set);

inline xtnt_status_t
xtnt_set_grow_eval(
    struct xtnt_node_set *set,
    size_t eval);

inline xtnt_status_t
xtnt_set_shrink(
    struct xtnt_node_set *set);

inline xtnt_status_t
xtnt_set_shrink_eval(
    struct xtnt_node_set *set,
    size_t eval);

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

#endif /* ifndef _XTNT_SET_COMMON_H_ */
