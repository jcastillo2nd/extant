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
 * @param[in] array The node set pointer to populate
 * @param[in] count The size of the array
 * @return node set pointer or NULL on error
 */
struct xtnt_node_set *xtnt_array_create(
        struct xtnt_node_set *array,
        xtnt_uint_t count)
{
    struct xtnt_node *created = malloc(sizeof(struct xtnt_node*) * count);
    if (created != NULL) {
        xtnt_int_t fail = pthread_mutex_lock(&(array->lock));
        if (fail) {
            XTNT_NODES_LOCK_SET_FAIL(array->state);
            return NULL;
        }
        array->link[XTNT_NODE_HEAD] = created;
        array->link[XTNT_NODE_TAIL] = &created[count - 1];
        array->count = count; 
        fail = pthread_mutex_unlock(&(array->lock));
        if (fail) {
            XTNT_NODES_LOCK_SET_FAIL(array->state);
        }
        return array; 
    }
    return NULL;
}

/**
 * @brief Destroy a finite sized array
 *
 * @param[in] array The node set pointer to delete
 */
void xtnt_array_destroy(
        struct xtnt_node_set *array)
{
    xtnt_int_t fail = pthread_mutex_lock(&(array->lock));
    if (fail) {
        XTNT_NODES_LOCK_SET_FAIL(array->state);
    }
    free(array->link[XTNT_NODE_HEAD]);
    array->link[XTNT_NODE_HEAD] = \
    array->link[XTNT_NODE_TAIL] = NULL;
    array->count = 0;
    fail = pthread_mutex_unlock(&(array->lock));
    if (fail) {
        XTNT_NODES_LOCK_SET_FAIL(array->state);
    }
}

/**
 * @brief Delete a node from an array
 *
 * @param[in] array the node set to delete from
 * @param[in] index the node to delete
 * @return node pointer of deleted or NULL
 *
 * @note Deleted nodes in an array are a NULL pointer
 */
struct xtnt_node *xtnt_array_delete(
        struct xtnt_node_set *array,
        xtnt_uint_t index)
{
    struct xtnt_node *deleted = NULL;
    xtnt_int_t fail = pthread_mutex_lock(&(array->lock));
    if (fail) {
        XTNT_NODES_LOCK_SET_FAIL(array->state);
    }
    if (index < array->count) {
        if (((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[index] != NULL) {
            deleted = ((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[index];
            ((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[index] = NULL;
        }
    }
    fail = pthread_mutex_unlock(&(array->lock));
    if (fail) {
        XTNT_NODES_LOCK_SET_FAIL(array->state);
    }
    return deleted;
}

/**
 * @brief Get an array member
 *
 * @param[in] array The node set to get from
 * @param[in] index The index to get
 * @return node or NULL on empty or invalid index
 */
struct xtnt_node *xtnt_array_get(
        struct xtnt_node_set *array,
        xtnt_uint_t index)
{
    struct xtnt_node *got = NULL;
    xtnt_int_t fail = pthread_mutex_lock(&(array->lock));
    if (fail) {
        XTNT_NODES_LOCK_SET_FAIL(array->state);
    }
    if (index < array->count) {
        if (((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[index] != NULL) {
             got = ((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[index];
        }
    }
    fail = pthread_mutex_unlock(&(array->lock));
    if (fail) {
        XTNT_NODES_LOCK_SET_FAIL(array->state);
    }
    return got;
}

/**
 * @brief Insert a node into an array
 *
 * @param[in] array The node set to insert into
 * @param[in] node The node to insert
 * @param[in] index The index to insert the node at
 * @return node pointer or NULL on failure
 */
struct xtnt_node *xtnt_array_insert(
        struct xtnt_node_set *array,
        struct xtnt_node *node,
        xtnt_uint_t index)
{
    xtnt_int_t fail = pthread_mutex_lock(&(array->lock));
    if (fail) {
        XTNT_NODES_LOCK_SET_FAIL(array->state);
    }
    if (index < array->count) {
            ((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[index] = node;
    }
    fail = pthread_mutex_unlock(&(array->lock));
    if (fail) {
        XTNT_NODES_LOCK_SET_FAIL(array->state);
    }
    return NULL;
}

/**
 * @brief Search array for node with key
 *
 * @param[in] array The array to search
 * @param[in] the key to check for
 * @return node on found or NULL on no match or error
 */
struct xtnt_node *xtnt_array_search(
        struct xtnt_node_set *array,
        xtnt_uint_t key)
{   
    struct xtnt_node *found = NULL;
    xtnt_int_t fail = pthread_mutex_lock(&(array->lock));
    if (fail) {
        XTNT_NODES_LOCK_SET_FAIL(array->state);
        return NULL;
    }
    for (xtnt_uint_t idx = 0; idx < array->count; idx++){
        if (((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[idx] != NULL &&
            ((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[idx]->key == key) {
            found = ((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[idx];
            break;
        }
    }
    fail = pthread_mutex_unlock(&(array->lock));
    if (fail) {
        XTNT_NODES_LOCK_SET_FAIL(array->state);
    }
    return found;
}

/**
 * @brief Search array for node matched by function
 *
 * @param[in] array The array to search
 * @param[in] test_fn The function to test with
 * @param[in] ctx The context the test uses to match
 * @return node on found or NULL
 */
struct xtnt_node *xtnt_array_search_fn(
        struct xtnt_node_set *array,
        void *test_fn,
        void *ctx)
{   
    struct xtnt_node *found = NULL;
    xtnt_uint_t (*test)(void *, void *) = test_fn;
    xtnt_int_t fail = pthread_mutex_lock(&(array->lock));
    if (fail) {
        XTNT_NODES_LOCK_SET_FAIL(array->state);
        return NULL;
    }
    for (xtnt_uint_t idx = 0; idx < array->count; idx++){
        if (((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[idx] != NULL &&
            test(ctx, ((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[idx])) {
            found = ((struct xtnt_node**) array->link[XTNT_NODE_HEAD])[idx];
            break;
        }
    }
    fail = pthread_mutex_unlock(&(array->lock));
    if (fail) {
        XTNT_NODES_LOCK_SET_FAIL(array->state);
    }
    return found;
}
