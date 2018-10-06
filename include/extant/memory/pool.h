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

#ifndef _XTNT_MEMORY_POOL_H_
#define _XTNT_MEMORY_POOL_H_

#include <extant/error.h>

#include <extant/memory/common.h>

xtnt_status_t
xtnt_mpool_allocate(
    struct xtnt_memory_object *pool,
    xtnt_uint_t count,
    void **allocation);

xtnt_status_t
xtnt_mpool_create(
    size_t size,
    xtnt_uint_t count,
    struct xtnt_memory_object **pool);

xtnt_status_t
xtnt_mpool_deallocate(
    struct xtnt_memory_object *pool,
    void **allocation);

xtnt_status_t
xtnt_mpool_destroy(
    struct xtnt_memory_object **pool);

#endif /* _XTNT_MEMORY_POOL_H_ */
