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

#include <extant/set/common.h>

/**
 * @brief Initialize a node with empty values
 *
 * @param[in] node Node to initialize
 * @param[in] key Key to set for Node
 * @param[in] value Node value pointer
 * @return xtnt_node pointer or NULL if error
 */
xtnt_int_t xtnt_node_initialize(
        struct xtnt_node *node,
        xtnt_uint_t key,
        void *value)
{
    xtnt_int_t fail = XTNT_SUCCESS;
    fail = pthread_mutex_init(&(node->lock), NULL);
    if (fail == 0){
        fail = pthread_mutex_lock(&(node->lock));
        if (fail == 0){
            node->key = key;
            node->value = value;
            node->link[0] = NULL;
            node->link[1] = NULL;
            node->link[2] = NULL;
            fail = pthread_mutex_unlock(&(node->lock));
        }
    }
    return fail;
}

/**
 * @brief Uninitialize a node
 *
 * @param[in] node Node to uninitialize
 * @return void pointer to value
 */
xtnt_int_t xtnt_node_uninitialize(
        struct xtnt_node *node)
{
    xtnt_int_t fail = XTNT_SUCCESS;
    fail = pthread_mutex_lock(&(node->lock));
    if (fail == 0){
        node->value = NULL;
        node->link[0] = NULL;
        node->link[1] = NULL;
        node->link[2] = NULL;
        node->key = 0;
        fail = pthread_mutex_unlock(&(node->lock));
        if (fail == 0){
            fail = pthread_mutex_destroy(&(node->lock));
        } else {
            XTNT_NODES_LOCK_SET_FAIL(node->state);
        }
    } else {
        XTNT_NODES_LOCK_SET_FAIL(node->state);
    }
    return fail;
}

/**
 * @brief Initialize a Node Set
 *
 * @param[in] set The Node Set to initialize
 */
xtnt_int_t xtnt_node_set_initialize(
        struct xtnt_node_set *set)
{
    xtnt_int_t fail = XTNT_SUCCESS;
    fail = pthread_mutex_init(&(set->lock), NULL);
    if (fail == 0) {
        fail = pthread_mutex_lock(&(set->lock));
        if (fail == 0){
            set->link[0] = NULL;
            set->link[1] = NULL;
            set->link[2] = NULL;
            set->count = 0;
            set->state = 0;
            fail = pthread_mutex_unlock(&(set->lock));
        } else {
            XTNT_NODES_LOCK_SET_FAIL(set->state);
        }
    } else {
        XTNT_NODES_LOCK_SET_FAIL(set->state);
    }
    return fail;
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
xtnt_int_t xtnt_node_set_uninitialize(
        struct xtnt_node_set *set)
{
    xtnt_int_t fail = XTNT_SUCCESS;
    fail = pthread_mutex_lock(&(set->lock));
    if (fail == 0){
        set->link[0] = NULL;
        set->link[1] = NULL;
        set->link[2] = NULL;
        set->count = 0;
        set->state = 0;
        fail = pthread_mutex_unlock(&(set->lock));
        if (fail == 0){
            fail = pthread_mutex_destroy(&(set->lock));
        } else {
            XTNT_NODES_LOCK_SET_FAIL(set->state);
        }
    } else {
        XTNT_NODES_LOCK_SET_FAIL(set->state);
    }
    return fail;
}
