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

#include <extant/set/tree/avltree.h>

xtnt_status_t
xtnt_avltree_delete(
    struct xtnt_node_set *tree,
    xtnt_uint_t key,
    struct xtnt_node **deleted);

xtnt_status_t
xtnt_avltree_insert(
    struct xtnt_node_set *tree,
    struct xtnt_node *node);

xtnt_status_t
xtnt_avltree_replace(
    struct xtnt_node_set *tree,
    struct xtnt_node *node,
    struct xtnt_node **replaced);

xtnt_status_t
xtnt_avltree_search(
    struct xtnt_node_set *tree,
    xtnt_uint_t key,
    struct xtnt_node **found);

xtnt_status_t
xtnt_avltree_search_fn(
    struct xtnt_node_set *tree,
    void *test_fn,
    void *ctx,
    struct xtnt_node **found);


