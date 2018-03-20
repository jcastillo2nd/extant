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

#ifndef _XTNT_SET_ARRAY_H_
#define _XTNT_SET_ARRAY_H_

#ifndef _XTNT_SET_COMMON_H_
#include <extant/set/common.h>
#endif /* _XTNT_SET_COMMON_H_ */

struct xtnt_node* xtnt_array_add(
        struct xtnt_node_set* array,
        struct xtnt_node* node);

struct xtnt_node* xtnt_array_delete(
        struct xtnt_node_set* array,
        xtnt_uint_t index);

struct xtnt_node* xtnt_array_get(
        struct xtnt_node_set* array,
        xtnt_uint_t index);

struct xtnt_node* xtnt_array_set(
        struct xtnt_node_set* array,
        struct xtnt_node* node,
        xtnt_uint_t index);

#endif /* ifndef _XTNT_SET_ARRAY_H_ */
