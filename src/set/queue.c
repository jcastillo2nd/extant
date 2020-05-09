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

#include <extant/set/queue.h>

/**
 * @brief Peek at the next entry in a queue
 *
 * @param[in] queue The xtnt_node_set to operate on
 * @param[out] node The xtnt_node or NULL if queue is empty
 * @retval XTNT_ESUCCESS on successful peek
 * @retval return value of `pthread_mutex_lock()` or `pthread_mutex_unlock()`
 * calls.
 */
xtnt_status_t
xtnt_queue_peek(
    struct xtnt_node_set *queue,
    struct xtnt_node **node)
{
    xtnt_status_t res = XTNT_EFAILURE;
    if ((res = pthread_mutex_lock(&(queue->lock))) == XTNT_ESUCCESS) {
        if (queue->link[XTNT_NODE_TAIL] != NULL) {
            *node = queue->link[XTNT_NODE_TAIL];
        }
        if ((res = pthread_mutex_unlock(&(queue->lock))) != XTNT_ESUCCESS) {
            XTNT_LOCK_SET_UNLOCK_FAIL(queue->state);
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(queue->state);
    }
    return res;
}

/**
 * @brief Remove the next entry in the queue
 *
 * @param[in] queue The `xtnt_node_set` to operate on
 * @param[out] node The xtnt_node or NULL if queue is empty
 * @retval XTNT_ESUCCESS on successful pop
 * @retval return value of `pthread_mutex_lock()` or `pthread_mutex_unlock()`
 */
xtnt_status_t
xtnt_queue_pop(
    struct xtnt_node_set *queue,
    struct xtnt_node **node)
{
    xtnt_status_t res = XTNT_EFAILURE;
    if ((res = pthread_mutex_lock(&(queue->lock))) == XTNT_ESUCCESS) {
        *node = queue->link[XTNT_NODE_TAIL];
        if (*node != NULL) {
            if ((uintptr_t) queue->link[XTNT_NODE_HEAD] ^
                (uintptr_t) queue->link[XTNT_NODE_TAIL]) {
                (*node)->link[XTNT_NODE_HEAD]->link[XTNT_NODE_TAIL] = NULL;
                queue->link[XTNT_NODE_TAIL] = (*node)->link[XTNT_NODE_HEAD];
            } else {
                queue->link[XTNT_NODE_TAIL] = NULL;
                queue->link[XTNT_NODE_HEAD] = NULL;
            }
            queue->count--;
        }
        if ((res = pthread_mutex_unlock(&(queue->lock))) == XTNT_ESUCCESS) {
            XTNT_LOCK_SET_UNLOCK_FAIL(queue->state);
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(queue->state);
    }
    return res;
}

/**
 * @brief Add an entry to the queue
 *
 * @param[in] queue The `xtnt_node_set` to operate on
 * @param[in] node The `xtnt_node` to add to the queue
 * @retval XTNT_ESUCCESS on successful push
 * @retval return value of `pthread_mutex_lock()` or `pthread_mutex_unlock()`
 */
xtnt_status_t
xtnt_queue_push(
    struct xtnt_node_set *queue,
    struct xtnt_node *node)
{
    xtnt_status_t res = XTNT_EFAILURE;
    if ((res = pthread_mutex_lock(&(queue->lock))) == XTNT_ESUCCESS) {
        if (queue->link[XTNT_NODE_HEAD] != NULL) {
            queue->link[XTNT_NODE_HEAD]->link[XTNT_NODE_HEAD] = node;
            node->link[XTNT_NODE_TAIL] = queue->link[XTNT_NODE_HEAD];
            queue->link[XTNT_NODE_HEAD] = node;
        } else {
            queue->link[XTNT_NODE_TAIL] = node;
            queue->link[XTNT_NODE_HEAD] = node;
        }
        queue->count++;
        if ((res = pthread_mutex_unlock(&(queue->lock))) != XTNT_ESUCCESS) {
            XTNT_LOCK_SET_UNLOCK_FAIL(queue->state);
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(queue->state);
    }
    return res;
}
