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

#ifndef _XTNT_DSO_H_
#define _XTNT_DSO_H_

#include <extant/error.h>

struct xtnt_dso
{
    const char *name;
    void *handle;
};

struct xtnt_dso_symbol
{
    const char *name;
    void *ptr;
};

xtnt_status_t
xtnt_dso_handle_create(
    struct xtnt_dso **handle);

xtnt_status_t
xtnt_dso_load(
    struct xtnt_dso *handle,
    const char *name);

xtnt_status_t
xtnt_dso_symbol(
    struct xtnt_dso *handle,
    struct xtnt_dso_symbol **symbol,
    const char *name);

xtnt_status_t
xtnt_dso_unload(
    struct xtnt_dso *handle);

#endif /* _XTNT_DSO_H_ */
