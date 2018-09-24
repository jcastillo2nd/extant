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

#include <extant/set/list.h>

/**
 * @brief Remove a node from the list
 *
 * @param[in] list The Node List to add to
 * @param[in] index The position in the list to remove
 * @return node removed or NULL on invalid index or error
 */
struct xtnt_node *xtnt_list_delete(
        struct xtnt_node_set *list,
        xtnt_uint_t index)
{
    struct xtnt_node *removed = NULL;
    xtnt_uint_t dir = 0;
    xtnt_int_t fail = pthread_mutex_lock(&(list->lock));
    if (fail) {
        XTNT_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    if (list->count > index) {
        // Array has index
        if (index < (list->count >> 1)) {
            // Start from head of list
            removed = list->link[XTNT_NODE_HEAD];
            dir = XTNT_NODE_TAIL;
        } else {
            // Start from tail of list
            removed = list->link[XTNT_NODE_TAIL];
            dir = XTNT_NODE_HEAD;
            index = list->count - (index + 1);
        }
        for (xtnt_uint_t idx = 0; idx < index; idx++) {
            removed = removed->link[dir];
        }
        if (removed->link[XTNT_NODE_HEAD] == NULL) {
            if (removed->link[XTNT_NODE_TAIL] != NULL) {
                // We are the head, and there are more nodes
                removed->link[XTNT_NODE_TAIL]->link[XTNT_NODE_HEAD] = NULL;
                list->link[XTNT_NODE_HEAD] = removed->link[XTNT_NODE_TAIL]; 
            } else {
                list->link[XTNT_NODE_HEAD] = NULL;
            }
        }
        if (removed->link[XTNT_NODE_TAIL] == NULL) {
            if (removed->link[XTNT_NODE_HEAD] != NULL) {
                // We are the tail, and there are more nodes
                removed->link[XTNT_NODE_HEAD]->link[XTNT_NODE_TAIL] = NULL;
                list->link[XTNT_NODE_TAIL] = removed->link[XTNT_NODE_HEAD];
            } else {
                list->link[XTNT_NODE_TAIL] = NULL;
            }
        }
        list->count--;
    }
    fail = pthread_mutex_unlock(&(list->lock));
    if (fail) {
        XTNT_LOCK_SET_FAIL(list->state);
    }
    return removed;
}

/**
 * @brief Add a node to the list
 *
 * @param[in] list The Node Set to add to
 * @param[in] node The Node to add
 * @return node added or NULL on error
 *
 * @warning
 * Do not re-add an existing member to the list. To attach the same value,
 * create a new node with the value, and add the new node instead.
 */
struct xtnt_node *xtnt_list_insert(
        struct xtnt_node_set *list,
        struct xtnt_node *node)
{
    xtnt_int_t fail = pthread_mutex_lock(&(list->lock));
    if (fail) {
        XTNT_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    // Are we special case 0 elements?
    if (list->count == 0){
        // We are the head and tail
        list->link[XTNT_NODE_HEAD] = node;
        list->link[XTNT_NODE_TAIL] = node;
    } else {
        // We have a new tail
        node->link[XTNT_NODE_TAIL] = list->link[XTNT_NODE_HEAD];
        // We are a new head to our tail
        list->link[XTNT_NODE_HEAD]->link[XTNT_NODE_HEAD] = node;
        // We are the new head
        list->link[XTNT_NODE_HEAD] = node;
    }
    list->count++;
    fail = pthread_mutex_unlock(&(list->lock));
    if (fail) {
        XTNT_LOCK_SET_FAIL(list->state);
    }   
    return node;
}

/**
 * @brief Set an list memeber
 *
 * @param[in] list The Node Set to update
 * @param[in] node The node to replace with
 * @param[in] index The index to update at
 * @return node that was replaced or NULL on invalid index or error
 *
 * @remark
 * Returns NULL on an empty set. To add an element to an empty list at index
 * 0, use `xtnt_list_insert()`.
 *
 * @remark
 * This function was implemented for completeness. The `xtnt_node` node itself
 * can have node.value updated to a new reference if `xtnt_list_search()`
 * returns a node from user code instead to re-use the node.
 *
 * @warn
 * An already existing member node must not be used. If you need to set the
 * same value at another index, use a new `xtnt_node` initialized with the
 * value instead.
 */
struct xtnt_node *xtnt_list_replace(
        struct xtnt_node_set *list,
        struct xtnt_node *node,
        xtnt_uint_t index)
{
    struct xtnt_node *found = NULL;
    xtnt_uint_t dir = 0;
    xtnt_int_t fail = pthread_mutex_lock(&(list->lock));
    if (fail) {
        XTNT_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    if (list->count > index) {
        // Array has index
        if (index < (list->count >> 1)) {
            // Start from head of list
            found = list->link[XTNT_NODE_HEAD];
            dir = XTNT_NODE_TAIL;
        } else {
            // Start from tail of list
            found = list->link[XTNT_NODE_TAIL];
            dir = XTNT_NODE_HEAD;
            index = list->count - (index + 1);
        }
        for (xtnt_uint_t idx = 0; idx < index; idx++) {
            found = found->link[dir];
        }
        node->link[XTNT_NODE_HEAD] = found->link[XTNT_NODE_HEAD];
        if (found->link[XTNT_NODE_HEAD] != NULL) {
            found->link[XTNT_NODE_HEAD]->link[XTNT_NODE_TAIL] = node;
        } else {
            list->link[XTNT_NODE_HEAD] = node; 
        }
        node->link[XTNT_NODE_TAIL] = found->link[XTNT_NODE_TAIL];
        if (found->link[XTNT_NODE_TAIL] != NULL) {
            found->link[XTNT_NODE_TAIL]->link[XTNT_NODE_HEAD] = node;
        } else {
            list->link[XTNT_NODE_HEAD] = node;
        }
    }
    fail = pthread_mutex_unlock(&(list->lock));
    if (fail) {
        XTNT_LOCK_SET_FAIL(list->state);
    }
    return found;
}

/**
 * @brief Get a list member
 *
 * @param[in] list The Node List to read
 * @param[in] index The Node index to retreive
 * @return node at index or NULL on invalid index or error
 */
struct xtnt_node *xtnt_list_get(
        struct xtnt_node_set *list,
        xtnt_uint_t index)
{
    struct xtnt_node *got = NULL;
    xtnt_uint_t dir = 0;
    xtnt_int_t fail = pthread_mutex_lock(&(list->lock));
    if (fail) {
        XTNT_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    if (list->count > index) {
        // Array has index
        if (index < (list->count >> 1)) {
            // Start from head of list
            got = list->link[XTNT_NODE_HEAD];
            dir = XTNT_NODE_TAIL;
        } else {
            // Start from tail of list
            got = list->link[XTNT_NODE_TAIL];
            dir = XTNT_NODE_HEAD;
            index = list->count - (index + 1);
        }
        for (xtnt_uint_t idx = 0; idx < index; idx++) {
            got = got->link[dir];
        }
    }
    fail = pthread_mutex_unlock(&(list->lock));
    if (fail) {
        XTNT_LOCK_SET_FAIL(list->state);
    }
    return got;
}

/**
 * @brief Search for key in list
 *
 * @param[in] list The Node Set to read through
 * @param[in] key The key to match
 * @return node on match or NULL
 */
struct xtnt_node *xtnt_list_search(
        struct xtnt_node_set *list,
        xtnt_uint_t key)
{
    struct xtnt_node *found = NULL;
    struct xtnt_node *test = NULL;
    xtnt_int_t fail = pthread_mutex_lock(&(list->lock));
    if (fail) {
        XTNT_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    test = list->link[XTNT_NODE_HEAD];
    do {
        if (test->key == key) {
            found = test;
            break;
        }
    } while (test->link[XTNT_NODE_TAIL] != NULL);
    fail = pthread_mutex_unlock(&(list->lock));
    if (fail) {
        XTNT_LOCK_SET_FAIL(list->state);
    }
    return found;
}

/**
 * @brief Search for key in list
 *
 * @param[in] list The Node Set to read through
 * @param[in] test_fn The test function to call on each node
 * @return node on match or NULL
 *
 * @remark The test should return 0 on match ( no difference ) or non-zero
 * on mismatch with the signature:
 *
 * @code
 * xtnt_uint_t callback(void *ctx, struct xtnt_node *node);
 * @endcode
 */
struct xtnt_node *xtnt_list_search_fn(
        struct xtnt_node_set *list,
        void *test_fn,
        void *ctx)
{
    struct xtnt_node *found = NULL;
    struct xtnt_node *testing = NULL;
    xtnt_uint_t (*test)(void *, struct xtnt_node *) = test_fn;
    xtnt_int_t fail = pthread_mutex_lock(&(list->lock));
    if (fail) {
        XTNT_LOCK_SET_FAIL(list->state);
        return NULL;
    }
    testing = list->link[XTNT_NODE_HEAD];
    do {
        if (test(ctx, testing)) {
            found = testing;
            break;
        }
    } while (testing->link[XTNT_NODE_TAIL] != NULL);
    fail = pthread_mutex_unlock(&(list->lock));
    if (fail) {
        XTNT_LOCK_SET_FAIL(list->state);
    }
    return found;
}
