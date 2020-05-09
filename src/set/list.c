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

#include <extant/set/list.h>

/**
 * @brief Remove a node from the list
 *
 * @param[in] list The Node List to add to
 * @param[in] index The position in the list to remove
 * @param[out] deleted node removed from list
 * @return node removed or NULL on invalid index or error
 */
xtnt_status_t
xtnt_list_delete(
    struct xtnt_node_set *list,
    xtnt_uint_t index,
    struct xtnt_node **deleted)
{
    xtnt_status_t res = XTNT_EFAILURE;
    xtnt_uint_t dir = XTNT_NODE_HEAD;
    if ((res = pthread_mutex_lock(&(list->lock))) == XTNT_ESUCCESS) {
        if (list->count > index) {
            if (index < (list->count >> 1)) {
                *deleted = list->link[XTNT_NODE_HEAD];
                dir = XTNT_NODE_TAIL;
            } else {
                *deleted = list->link[XTNT_NODE_TAIL];
                index = list->count - (index + 1);
            }
            for (xtnt_uint_t idx = 0; idx < index; idx++) {
                *deleted = (*deleted)->link[dir];
            }
            if ((*deleted)->link[XTNT_NODE_HEAD] == NULL) {
                if ((*deleted)->link[XTNT_NODE_TAIL] != NULL) {
                    (*deleted)->link[XTNT_NODE_TAIL]->link[XTNT_NODE_HEAD] = NULL;
                    list->link[XTNT_NODE_HEAD] = (*deleted)->link[XTNT_NODE_TAIL]; 
                } else {
                    list->link[XTNT_NODE_HEAD] = NULL;
                }
            }
            if ((*deleted)->link[XTNT_NODE_TAIL] == NULL) {
                if ((*deleted)->link[XTNT_NODE_HEAD] != NULL) {
                    (*deleted)->link[XTNT_NODE_HEAD]->link[XTNT_NODE_TAIL] = NULL;
                    list->link[XTNT_NODE_TAIL] = (*deleted)->link[XTNT_NODE_HEAD];
                } else {
                    list->link[XTNT_NODE_TAIL] = NULL;
                }
            }
            list->count--;
        } else {
            *deleted = NULL;
        }
        if ((res = pthread_mutex_unlock(&(list->lock))) != XTNT_ESUCCESS) {
            XTNT_LOCK_SET_UNLOCK_FAIL(list->state);
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(list->state);
    }
    return res;
}

/**
 * @brief Get a list member
 *
 * @param[in] list The Node List to read
 * @param[in] index The Node index to retreive
 * @param[out] node The Node found at index or NULL
 * @return node at index or NULL on invalid index or error
 */
xtnt_status_t
xtnt_list_get(
    struct xtnt_node_set *list,
    xtnt_uint_t index,
    struct xtnt_node **node)
{
    xtnt_status_t res = XTNT_EFAILURE;
    xtnt_uint_t dir = XTNT_NODE_HEAD;
    if ((res = pthread_mutex_lock(&(list->lock))) == XTNT_ESUCCESS) {
        if (list->count > index) {
            if (index < (list->count >> 1)) {
                *node = list->link[XTNT_NODE_HEAD];
                dir = XTNT_NODE_TAIL;
            } else {
                *node = list->link[XTNT_NODE_TAIL];
                index = list->count - (index + 1);
            }
            for (xtnt_uint_t idx = 0; idx < index; idx++) {
                *node = (*node)->link[dir];
            }
        } else {
            *node = NULL;
        }
        if ((res = pthread_mutex_unlock(&(list->lock))) != XTNT_ESUCCESS) {
            XTNT_LOCK_SET_UNLOCK_FAIL(list->state);
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(list->state);
    }
    return res;
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
xtnt_status_t
xtnt_list_insert(
    struct xtnt_node_set *list,
    struct xtnt_node *node)
{
    xtnt_status_t res = XTNT_EFAILURE;
    if ((res = pthread_mutex_lock(&(list->lock))) == XTNT_ESUCCESS) {
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
        if ((res = pthread_mutex_unlock(&(list->lock))) != XTNT_ESUCCESS) {
            XTNT_LOCK_SET_UNLOCK_FAIL(list->state);
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(list->state);
    }
    return res;
}

/**
 * @brief Set an list memeber
 *
 * @param[in] list The Node Set to update
 * @param[in] node The node to replace with
 * @param[in] index The index to update at
 * @param[out] replaced The Node that was replaced
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
 * @warning
 * An already existing member node must not be used. If you need to set the
 * same value at another index, use a new `xtnt_node` initialized with the
 * same value instead.
 */
xtnt_status_t
xtnt_list_replace(
    struct xtnt_node_set *list,
    struct xtnt_node *node,
    xtnt_uint_t index,
    struct xtnt_node **replaced)
{
    xtnt_status_t res = XTNT_EFAILURE;
    xtnt_uint_t dir = XTNT_NODE_HEAD;
    if ((res = pthread_mutex_lock(&(list->lock))) == XTNT_ESUCCESS) {
        if (list->count > index) {
            if (index < (list->count >> 1)) {
                *replaced = list->link[XTNT_NODE_HEAD];
                dir = XTNT_NODE_TAIL;
            } else {
                *replaced = list->link[XTNT_NODE_TAIL];
                index = list->count - (index + 1);
            }
            for (xtnt_uint_t idx = 0; idx < index; idx++) {
                *replaced = (*replaced)->link[dir];
            }
            node->link[XTNT_NODE_HEAD] = (*replaced)->link[XTNT_NODE_HEAD];
            if ((*replaced)->link[XTNT_NODE_HEAD] != NULL) {
                (*replaced)->link[XTNT_NODE_HEAD]->link[XTNT_NODE_TAIL] = node;
            } else {
                list->link[XTNT_NODE_HEAD] = node; 
            }
            node->link[XTNT_NODE_TAIL] = (*replaced)->link[XTNT_NODE_TAIL];
            if ((*replaced)->link[XTNT_NODE_TAIL] != NULL) {
                (*replaced)->link[XTNT_NODE_TAIL]->link[XTNT_NODE_HEAD] = node;
            } else {
                list->link[XTNT_NODE_HEAD] = node;
            }
        } else {
            *replaced = NULL;
        }
        if ((res = pthread_mutex_unlock(&(list->lock))) != XTNT_ESUCCESS) {
            XTNT_LOCK_SET_UNLOCK_FAIL(list->state);
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(list->state);
    }
    return res;
}

/**
 * @brief Search for key in list
 *
 * @param[in] list The Node Set to read through
 * @param[in] key The key to match
 * @param[out] found Reference to matching Node or NULL
 * @return node on match or NULL
 */
xtnt_status_t
xtnt_list_search(
    struct xtnt_node_set *list,
    xtnt_uint_t key,
    struct xtnt_node **found)
{
    xtnt_status_t res = XTNT_EFAILURE;
    if ((res = pthread_mutex_lock(&(list->lock))) == XTNT_ESUCCESS) {
        *found = list->link[XTNT_NODE_HEAD];
        do {
            if (*found == NULL || (*found)->key == key) {
                break;
            }
        } while ((*found = (*found)->link[XTNT_NODE_TAIL]));
        if ((res = pthread_mutex_unlock(&(list->lock))) != XTNT_ESUCCESS) {
            XTNT_LOCK_SET_UNLOCK_FAIL(list->state);
        }
        if ((res = pthread_mutex_unlock(&(list->lock))) != XTNT_ESUCCESS) {
            XTNT_LOCK_SET_UNLOCK_FAIL(list->state);
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(list->state);
    }
    return res;
}

/**
 * @brief Search for key in list
 *
 * @param[in] list The Node Set to read through
 * @param[in] test_fn The test function to call on each node
 * @param[in] ctx Context passed into the search function
 * @return node on match or NULL
 *
 * @remark The test should return 0 on match ( no difference ) or non-zero
 * on mismatch with the signature:
 *
 * @code
 * xtnt_uint_t callback(void *ctx, struct xtnt_node *node);
 * @endcode
 *
 * @todo Implement tests for xtnt_list_search_fn
 */
xtnt_status_t
xtnt_list_search_fn(
    struct xtnt_node_set *list,
    void *test_fn,
    void *ctx,
    struct xtnt_node **found)
{
    xtnt_status_t res = XTNT_EFAILURE;
    xtnt_uint_t (*test)(void *, struct xtnt_node *) = test_fn;
    if ((res = pthread_mutex_lock(&(list->lock))) == XTNT_ESUCCESS) {
        *found = list->link[XTNT_NODE_HEAD];
        do {
            if (*found == NULL || (test(ctx, *found) != 0)) {
                break;
            }
        } while ((*found = (*found)->link[XTNT_NODE_TAIL]));
        if ((res = pthread_mutex_unlock(&(list->lock))) != XTNT_ESUCCESS) {
            XTNT_LOCK_SET_UNLOCK_FAIL(list->state);
        }
        if ((res = pthread_mutex_unlock(&(list->lock))) != XTNT_ESUCCESS) {
            XTNT_LOCK_SET_UNLOCK_FAIL(list->state);
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(list->state);
    }
    return res;
}
