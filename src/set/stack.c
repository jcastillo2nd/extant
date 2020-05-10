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

#include <extant/set/stack.h>

/**
 * @brief Peek at the next entry in a stack
 *
 * @param[in] stack The xtnt_node_set to operate on
 * @param[out] node The xtnt_node or NULL if stack is empty
 * @retval XTNT_ESUCCESS on successful peek
 * @retval return value of `pthread_mutex_lock()` or `pthread_mutex_unlock()`
 * calls.
 */
xtnt_status_t
xtnt_stack_peek(
    struct xtnt_node_set *stack,
    struct xtnt_node **node)
{
    xtnt_status_t res = XTNT_EFAILURE;
    if ((res = pthread_mutex_lock(&(stack->lock))) == XTNT_ESUCCESS) {
        if (stack->root.link[XTNT_NODE_HEAD] != NULL) {
            *node = stack->root.link[XTNT_NODE_HEAD];
        }
        if ((res = pthread_mutex_unlock(&(stack->lock))) != XTNT_ESUCCESS) {
            XTNT_LOCK_SET_UNLOCK_FAIL(stack->root.state);
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(stack->root.state);
    }
    return res;
}

/**
 * @brief Remove the next entry in the stack
 *
 * @param[in] stack The `xtnt_node_set` to operate on
 * @param[out] node The xtnt_node or NULL if stack is empty
 * @retval XTNT_ESUCCESS on successful pop
 * @retval return value of `pthread_mutex_lock()` or `pthread_mutex_unlock()`
 */
xtnt_status_t
xtnt_stack_pop(
    struct xtnt_node_set *stack,
    struct xtnt_node **node)
{
    xtnt_status_t res = XTNT_EFAILURE;
    if ((res = pthread_mutex_lock(&(stack->lock))) == XTNT_ESUCCESS) {
        *node = stack->root.link[XTNT_NODE_HEAD];
        if (*node != NULL) {
            // Use of size_t to compare pointers lead to any bugs?
            if ((size_t) stack->root.link[XTNT_NODE_HEAD] ^ (size_t) stack->root.link[XTNT_NODE_TAIL]) {
                (*node)->link[XTNT_NODE_TAIL]->link[XTNT_NODE_HEAD] = NULL;
                stack->root.link[XTNT_NODE_HEAD] = (*node)->link[XTNT_NODE_TAIL];
            } else {
                stack->root.link[XTNT_NODE_HEAD] = NULL;
                stack->root.link[XTNT_NODE_TAIL] = NULL;
            }
            stack->count--;
        }
        if ((res = pthread_mutex_unlock(&(stack->lock))) == XTNT_ESUCCESS) {
            XTNT_LOCK_SET_UNLOCK_FAIL(stack->root.state);
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(stack->root.state);
    }
    return res;
}

/**
 * @brief Add an entry to the stack
 *
 * @param[in] stack The `xtnt_node_set` to operate on
 * @param[in] node The `xtnt_node` to add to the stack
 * @retval XTNT_ESUCCESS on successful push
 * @retval return value of `pthread_mutex_lock()` or `pthread_mutex_unlock()`
 */
xtnt_status_t
xtnt_stack_push(
    struct xtnt_node_set *stack,
    struct xtnt_node *node)
{
    xtnt_status_t res = XTNT_EFAILURE;
    if ((res = pthread_mutex_lock(&(stack->lock))) == XTNT_ESUCCESS) {
        if (stack->root.link[XTNT_NODE_TAIL] != NULL) {
            stack->root.link[XTNT_NODE_HEAD]->link[XTNT_NODE_HEAD] = node;
            node->link[XTNT_NODE_TAIL] = stack->root.link[XTNT_NODE_HEAD];
            stack->root.link[XTNT_NODE_HEAD] = node;
        } else {
            stack->root.link[XTNT_NODE_TAIL] = node;
            stack->root.link[XTNT_NODE_HEAD] = node;
        }
        stack->count++;
        if ((res = pthread_mutex_unlock(&(stack->lock))) != XTNT_ESUCCESS) {
            XTNT_LOCK_SET_UNLOCK_FAIL(stack->root.state);
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(stack->root.state);
    }
    return res;
}
