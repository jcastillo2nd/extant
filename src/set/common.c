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
 * @brief Initialize a node with empty values
 *
 * @param[in] node Node to initialize
 * @param[in] key Key to set for Node
 * @param[in] quirk Namespace key for node reference
 * @param[in] value Node value pointer
 * @return xtnt_node pointer or NULL if error
 */
xtnt_status_t
xtnt_node_initialize(
    struct xtnt_node *node,
    xtnt_uint_t key,
    xtnt_uint_t quirk,
    void *value)
{
    xtnt_status_t res = XTNT_EFAILURE;
    if ((res = pthread_mutex_init(&(node->lock), NULL)) == XTNT_ESUCCESS) {
        if ((res = pthread_mutex_lock(&(node->lock))) == XTNT_ESUCCESS) {
            node->value = value;
            node->link[0] = NULL;
            node->link[1] = NULL;
            node->link[2] = NULL;
            node->key = key;
            node->state = XTNT_ESUCCESS;
            node->quirk = quirk;
            if ((res = pthread_mutex_unlock(&(node->lock))) != XTNT_ZERO) {
                XTNT_LOCK_SET_UNLOCK_FAIL(node->state);
            }
        } else {
            XTNT_LOCK_SET_LOCK_FAIL(node->state);
        }
    } else {
        XTNT_LOCK_SET_INIT_FAIL(node->state);
    }
    return res;
}

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
            dst->link[XTNT_NODE_HEAD] = src->link[XTNT_NODE_HEAD];
            dst->link[XTNT_NODE_MIDDLE] = src->link[XTNT_NODE_MIDDLE];
            dst->link[XTNT_NODE_TAIL] = src->link[XTNT_NODE_TAIL];
            dst->count = src->count;
            XTNT_STATE_SET_VALUE(dst->state, XTNT_ZERO);
            if ((res = pthread_mutex_unlock(&(dst->lock))) == XTNT_ESUCCESS) {
                if ((res = pthread_mutex_unlock(&(src->lock))) != XTNT_ESUCCESS) {
                    XTNT_LOCK_SET_UNLOCK_FAIL(src->state);
                }
            } else {
                XTNT_LOCK_SET_UNLOCK_FAIL(dst->state);
            }
        } else {
            XTNT_LOCK_SET_LOCK_FAIL(dst->state);
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(src->state);
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
            set->link[0] = NULL;
            set->link[1] = NULL;
            set->link[2] = NULL;
            set->count = XTNT_ZERO;
            set->state = XTNT_ZERO;
            if ((res = pthread_mutex_unlock(&(set->lock))) != XTNT_ZERO) {
                XTNT_LOCK_SET_UNLOCK_FAIL(set->state);
            }
        } else {
            XTNT_LOCK_SET_LOCK_FAIL(set->state);
        }
    } else {
        XTNT_LOCK_SET_INIT_FAIL(set->state);
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
                XTNT_LOCK_SET_DESTROY_FAIL(set->state);
            }
        } else {
            XTNT_LOCK_SET_UNLOCK_FAIL(set->state);
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(set->state);
    }
    return res;
}

/**
 * @brief Uninitialize a node
 *
 * @param[in] node Node to uninitialize
 */
xtnt_status_t
xtnt_node_uninitialize(
    struct xtnt_node *node)
{
    xtnt_status_t res = XTNT_EFAILURE;
    if ((res = pthread_mutex_lock(&(node->lock))) == XTNT_ZERO) {
        if ((res = pthread_mutex_unlock(&(node->lock))) == XTNT_ZERO) {
            if ((res = pthread_mutex_destroy(&(node->lock))) != XTNT_ZERO) {
                XTNT_LOCK_SET_DESTROY_FAIL(node->state);
            }
        } else {
            XTNT_LOCK_SET_UNLOCK_FAIL(node->state);
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(node->state);
    }
    return res;
}

xtnt_status_t
xtnt_set_index(
    struct xtnt_node_set *s,
    xtnt_int_t index,
    struct xtnt_node **found)
{
    return s->fn->index((struct xtnt_node_set_if*) s, index, found);
}
