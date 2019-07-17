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

#include <extant/set/array.h>

/**
 * @brief Create a finite sized array
 *
 * @param[in] count The size of the array
 * @param[out] array Pointer to array reference to set
 * @retval XTNT_ESUCCESS on allocation and initialization
 * @retval errno on malloc
 * @retval of xtnt_node_set_initialize
 *
 * @todo Fix function documentation, implementation and create unit tests.
 */
xtnt_status_t
xtnt_array_create(
    xtnt_uint_t count,
    struct xtnt_node_set **array)
{
    xtnt_status_t res = XTNT_ESUCCESS;
    struct xtnt_node_set *marray = malloc(sizeof(struct xtnt_node_set) + (sizeof(struct xtnt_node) * count));
    struct xtnt_node *narray = (struct xtnt_node *) (marray + sizeof(struct xtnt_node_set));

    if (marray != NULL) {
        if ((res = xtnt_node_set_initialize(marray)) == XTNT_ESUCCESS) {
            if ((res = pthread_mutex_lock(&(marray->lock))) == XTNT_ESUCCESS) {
                marray->link[XTNT_NODE_HEAD] = narray;
                marray->link[XTNT_NODE_TAIL] = narray + (count - 1);
                marray->count = count;
                if ((res = pthread_mutex_unlock(&(marray->lock))) != XTNT_ESUCCESS) {
                    XTNT_LOCK_SET_UNLOCK_FAIL(marray->state);
                }
                *array = marray;
            } else {
                XTNT_LOCK_SET_LOCK_FAIL(marray->state);
            }
        } else {
            free(marray);
            *array = NULL;
        }
    } else {
        res = errno;
    }
    return res;
}

/**
 * @brief Delete a node from an array
 *
 * @param[in] array the node set to delete from
 * @param[in] index the node to delete
 * @param[out] node pointer of deleted node ( can be NULL )
 * @return node pointer of deleted or NULL
 *
 * @note Deleted nodes in an array are a NULL pointer
 *
 * @todo Fix function documentation, implementation and create unit tests.
 */
xtnt_status_t
xtnt_array_delete(
    struct xtnt_node_set *array,
    xtnt_uint_t index,
    struct xtnt_node **node)
{
    *node = NULL;
    xtnt_int_t fail = pthread_mutex_lock(&(array->lock));
    if (fail) {
        XTNT_LOCK_SET_LOCK_FAIL(array->state);
    }
    if (index < array->count) {
        if (((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[index] != NULL) {
            *node = ((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[index];
            ((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[index] = NULL;
        }
    }
    fail = pthread_mutex_unlock(&(array->lock));
    if (fail) {
        XTNT_LOCK_SET_UNLOCK_FAIL(array->state);
    }
    return fail;
}

/**
 * @brief Destroy a finite sized array
 *
 * @param[out] array Pointer to array reference
 *
 * @note This should only be called for arrays created with the 
 * `xtnt_array_create()` function. 
 *
 * @warning **Node structure data in the array is
 * freed.**
 */
xtnt_status_t
xtnt_array_destroy(
    struct xtnt_node_set **array)
{
    xtnt_status_t status = XTNT_EFAILURE;
    struct xtnt_node_set *a = *array;
    if ((status = pthread_mutex_lock(&(a->lock))) == XTNT_ESUCCESS) {
        if ((status = pthread_mutex_unlock(&(a->lock))) == XTNT_ESUCCESS){
            free(a);
            *array = NULL;
        } else {
            XTNT_LOCK_SET_UNLOCK_FAIL(a->state);
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(a->state);
        status = XTNT_EFAILURE;
    }
    return status;
}

/**
 * @brief Get an array member
 *
 * @param[in] array The node set to get from
 * @param[in] index The index to get
 * @param[out] node Pointer to node pointer retrieved
 * @return node or NULL on empty or invalid index
 *
 * @todo Fix function documentation, implementation and create unit tests.
 */
xtnt_status_t
xtnt_array_get(
    struct xtnt_node_set *array,
    xtnt_uint_t index,
    struct xtnt_node **node)
{
    xtnt_int_t fail = pthread_mutex_lock(&(array->lock));
    if (fail) {
        XTNT_LOCK_SET_LOCK_FAIL(array->state);
    }
    if (index < array->count) {
        if (((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[index] != NULL) {
             *node = ((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[index];
        }
    }
    fail = pthread_mutex_unlock(&(array->lock));
    if (fail) {
        XTNT_LOCK_SET_UNLOCK_FAIL(array->state);
    }
    return fail;
}

/**
 * @brief Insert a node into an array
 *
 * @param[in] array The node set to insert into
 * @param[in] node The node to insert
 * @param[in] index The index to insert the node at
 * @return node pointer or NULL on failure
 *
 * @todo Fix function documentation, implementation and create unit tests.
 */
xtnt_status_t
xtnt_array_insert(
    struct xtnt_node_set *array,
    struct xtnt_node *node,
    xtnt_uint_t index)
{
    xtnt_int_t fail = XTNT_ESUCCESS;
    fail = pthread_mutex_lock(&(array->lock));
    if (fail == 0) {
        if (index < array->count) {
                ((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[index] = node;
        } else {
            fail = EINVAL;
        }
        xtnt_int_t fail_unlock = pthread_mutex_unlock(&(array->lock));
        if (fail_unlock){
            XTNT_LOCK_SET_UNLOCK_FAIL(array->state);
            fail = fail_unlock;
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(array->state);
    }
    return fail;
}

/**
 * @brief Search array for node with key
 *
 * @param[in] array The array to search
 * @param[in] key the key to check for
 * @param[out] node Node pointer to set on match
 * @return node on found or NULL on no match or error
 *
 * @todo Fix function documentation, implementation and create unit tests.
 */
xtnt_status_t
xtnt_array_search(
    struct xtnt_node_set *array,
    xtnt_uint_t key,
    struct xtnt_node **node)
{   
    xtnt_int_t fail = pthread_mutex_lock(&(array->lock));
    if (fail) {
        XTNT_LOCK_SET_LOCK_FAIL(array->state);
        return fail;
    }
    for (xtnt_uint_t idx = 0; idx < array->count; idx++){
        if (((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[idx] != NULL &&
            ((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[idx]->key == key) {
            *node = ((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[idx];
            break;
        }
    }
    fail = pthread_mutex_unlock(&(array->lock));
    if (fail) {
        XTNT_LOCK_SET_UNLOCK_FAIL(array->state);
    }
    return fail;
}

/**
 * @brief Search array for node matched by function
 *
 * @param[in] array The array to search
 * @param[in] test_fn The function to test with
 * @param[in] ctx The context the test uses to match
 * @param[out] node Node pointer to set on match
 * @return node on found or NULL
 *
 * @todo Fix function documentation, implementation and create unit tests.
 */
xtnt_status_t
xtnt_array_search_fn(
    struct xtnt_node_set *array,
    void *test_fn,
    void *ctx,
    struct xtnt_node **node)
{   
    xtnt_uint_t (*test)(void *, void *) = test_fn;
    xtnt_int_t fail = pthread_mutex_lock(&(array->lock));
    if (fail) {
        XTNT_LOCK_SET_LOCK_FAIL(array->state);
        return fail;
    }
    for (xtnt_uint_t idx = 0; idx < array->count; idx++){
        if (((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[idx] != NULL &&
            test(ctx, ((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[idx])) {
            *node = ((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[idx];
            break;
        }
    }
    fail = pthread_mutex_unlock(&(array->lock));
    if (fail) {
        XTNT_LOCK_SET_UNLOCK_FAIL(array->state);
    }
    return fail;
}
