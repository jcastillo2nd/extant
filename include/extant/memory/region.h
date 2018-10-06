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
xtnt_mregion_allocate(
    struct xtnt_memory_object *region,
    size_t size,
    void **allocation);

xtnt_status_t
xtnt_mregion_create(
    size_t size,
    struct xtnt_memory_object **region);

xtnt_status_t
xtnt_mregion_deallocate(
    struct xtnt_memory_object *region,
    void **allocation);

xtnt_status_t
xtnt_mregion_destroy(
    struct xtnt_memory_object **region);

#endif /* _XTNT_MEMORY_POOL_H_ */
