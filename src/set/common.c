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

#include <extant/set/common.h>

/**
 * @brief Copy a node set
 *
 * @param[in] src The source node set
 * @param[in] dst The destination node set
 * @retval error on pthread lock
 */
xtnt_status_t
xtnt_node_set_copy(
    struct xtnt_node_set *src,
    struct xtnt_node_set *dst)
{
    xtnt_status_t res = XTNT_EFAILURE;
    if ((res = pthread_mutex_lock(&(src->lock))) == XTNT_ESUCCESS) {
        if ((res = pthread_mutex_lock(&(dst->lock))) == XTNT_ESUCCESS) {
            dst->root.link[XTNT_NODE_HEAD] = src->root.link[XTNT_NODE_HEAD];
            dst->root.link[XTNT_NODE_MIDDLE] = src->root.link[XTNT_NODE_MIDDLE];
            dst->root.link[XTNT_NODE_TAIL] = src->root.link[XTNT_NODE_TAIL];
            dst->count = src->count;
            XTNT_STATE_SET_VALUE(dst->root.state, XTNT_ZERO);
            if ((res = pthread_mutex_unlock(&(dst->lock))) == XTNT_ESUCCESS) {
                if ((res = pthread_mutex_unlock(&(src->lock))) != XTNT_ESUCCESS) {
                    XTNT_LOCK_SET_UNLOCK_FAIL(src->root.state);
                }
            } else {
                XTNT_LOCK_SET_UNLOCK_FAIL(dst->root.state);
            }
        } else {
            XTNT_LOCK_SET_LOCK_FAIL(dst->root.state);
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(src->root.state);
    }
    return res;
}

/**
 * @brief Initialize a Node Set
 *
 * @param[in] set The Node Set to initialize
 */
xtnt_status_t
xtnt_node_set_initialize(
    struct xtnt_node_set *set)
{
    xtnt_status_t res = XTNT_EFAILURE;
    if ((res = pthread_mutex_init(&(set->lock), NULL)) == XTNT_ZERO) {
        if ((res = pthread_mutex_lock(&(set->lock))) == XTNT_ZERO) {
            set->root.link[0] = NULL;
            set->root.link[1] = NULL;
            set->root.link[2] = NULL;
            set->count = XTNT_ZERO;
            set->root.state = XTNT_ZERO;
            if ((res = pthread_mutex_unlock(&(set->lock))) != XTNT_ZERO) {
                XTNT_LOCK_SET_UNLOCK_FAIL(set->root.state);
            }
        } else {
            XTNT_LOCK_SET_LOCK_FAIL(set->root.state);
        }
    } else {
        XTNT_LOCK_SET_INIT_FAIL(set->root.state);
    }
    return res;
}

/**
 * @brief Uninitialize a Node Set
 *
 * @param[in] set The Node Set to uninitialize
 * @return xtnt_node_list point or NULL on error
 *
 * @warning
 * This destroys the mutex on the object. There must be no current operations
 * executed on this tree, or tree is uninitialized but NULL returned.
 */
xtnt_status_t
xtnt_node_set_uninitialize(
    struct xtnt_node_set *set)
{
    xtnt_status_t res = XTNT_EFAILURE;
    if ((res = pthread_mutex_lock(&(set->lock))) == XTNT_ZERO) {
        if ((res = pthread_mutex_unlock(&(set->lock))) == XTNT_ZERO) {
            if ((res = pthread_mutex_destroy(&(set->lock))) != XTNT_ZERO) {
                XTNT_LOCK_SET_DESTROY_FAIL(set->root.state);
            }
        } else {
            XTNT_LOCK_SET_UNLOCK_FAIL(set->root.state);
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(set->root.state);
    }
    return res;
}

extern inline xtnt_status_t
xtnt_set_index(
    struct xtnt_node_set *set,
    xtnt_int_t index,
    struct xtnt_node **found)
{
    return set->fn->index(set, index, found);
}

extern inline xtnt_status_t
xtnt_set_search(
    struct xtnt_node_set *set,
    xtnt_uint_t key,
    struct xtnt_node **found)
{
    return set->fn->search(set, key, found);
}

extern inline xtnt_status_t
xtnt_set_search_fn(
    struct xtnt_node_set *set,
    void *fn,
    void *needle,
    struct xtnt_node **found)
{
    return set->fn->search_fn(set, fn, needle, found);
}

extern inline xtnt_status_t
xtnt_set_first(
    struct xtnt_node_set *set,
    struct xtnt_node **first)
{
    return set->fn->first(set, first);
}

extern inline xtnt_status_t
xtnt_set_last(
    struct xtnt_node_set *set,
    struct xtnt_node **last)
{
    return set->fn->last(set, last);
}

extern inline xtnt_status_t
xtnt_set_peek(
    struct xtnt_node_set *set,
    struct xtnt_node **peek)
{
    return set->fn->peek(set, peek);
}

extern inline xtnt_status_t
xtnt_set_root(
    struct xtnt_node_set *set,
    struct xtnt_node **root)
{
    return set->fn->root(set, root);
}

extern inline xtnt_status_t
xtnt_set_state(
    struct xtnt_node_set *set,
    xtnt_uint_t **state)
{
    return set->state(set, state);
}

extern inline xtnt_status_t
xtnt_set_insert(
    struct xtnt_node_set *set,
    struct xtnt_node *node)
{
    return set->fn->insert(set, node);
}

extern inline xtnt_status_t
xtnt_set_insert_at(
    struct xtnt_node_set *set,
    xtnt_uint_t index,
    struct xtnt_node *node,
    struct xtnt_node **replaced)
{
    return set->fn->insert_at(set, index, node, replaced);
}

extern inline xtnt_status_t
xtnt_set_push(
    struct xtnt_node_set *set,
    struct xtnt_node *node)
{
    set->fn->push(set, node);
}

extern inline xtnt_status_t
xtnt_set_remove(
    struct xtnt_node_set *set,
    struct xtnt_node *node)
{
    return set->fn->remove(set, node);
}

xtnt_status_t
extern inline xtnt_set_remove_at(
    struct xtnt_node_set *set,
    xtnt_uint_t index,
    struct xtnt_node **removed)
{
    return set->fn->remove_at(set, index, removed);
}

extern inline xtnt_status_t
xtnt_set_pop(
    struct xtnt_node_set *set,
    struct xtnt_node **node)
{
    return set->fn->pop(set, node);
}

extern inline xtnt_status_t
xtnt_set_sort(
    struct xtnt_node_set *set)
{
    return set->fn->sort(set);
}

extern inline xtnt_status_t
xtnt_set_sort_reverse(
    struct xtnt_node_set *set)
{
    return set->fn->sort_reverse(set);
}

extern inline xtnt_status_t
xtnt_set_sort_fn(
    struct xtnt_node_set *set,
    void *fn)
{
    return set->fn->sort_fn(set, fn);
}

extern inline xtnt_status_t
xtnt_set_grow(
    struct xtnt_node_set *set)
{
    return set->fn->grow(set);
}

extern inline xtnt_status_t
xtnt_set_grow_eval(
    struct xtnt_node_set *set,
    size_t eval)
{
    return set->fn->grow_eval(set, eval);
}

extern inline xtnt_status_t
xtnt_set_shrink(
    struct xtnt_node_set *set)
{
    return set->fn->shrink(set);
}

extern inline xtnt_status_t
xtnt_set_shrink_eval(
    struct xtnt_node_set *set,
    size_t eval)
{
    return set->fn->shrink_eval(set, eval);
}
