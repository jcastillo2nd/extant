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
    void **ptr = &node->value; // Intentionally discard const;
    node->key = key;
    node->link[0] = NULL;
    node->link[1] = NULL;
    node->link[2] = NULL;
    node->quirk = quirk;
    node->state = XTNT_ESUCCESS;
    *ptr = value;
    return XTNT_ESUCCESS;
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
    void **ptr = &node->value;
    node->key = node->quirk = 0;
    node->link[0] = node->link[1] = node->link[2] = NULL;
    node->state = XTNT_EFAILURE;
    *ptr = NULL;
    return XTNT_ESUCCESS;
}
