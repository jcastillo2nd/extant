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

#ifndef _XTNT_COMMON_H_
#define _XTNT_COMMON_H_

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>

#if HAVE_LONG_LONG_INT == 1

#define XTNT_INT_MAX LLONG_MAX
#define XTNT_UINT_MAX ULLONG_MAX
typedef long long int xtnt_int_t;
typedef unsigned long long int xtnt_uint_t;

#elif HAVE_LONG_INT == 1

#define XTNT_INT_MAX LONG_MAX
#define XTNT_UINT_MAX ULONG_MAX
typedef long int xtnt_int_t;
typedef unsigned long int xtnt_uint_t;

#else

#define XTNT_INT_MAX INT_MAX
#define XTNT_UINT_MAX UINT_MAX
typedef int xtnt_int_t;
typedef unsigned int xtnt_uint_t;

#endif /* ifdef HAVE_LONG_LONG_INT */

#if HAVE_LONG_DOUBLE == 1

typedef long double xtnt_real_t;

#elif HAVE_DOUBLE == 1

typedef double xtnt_real_t;

#else

typedef float xtnt_real_t;

#endif /* HAVE_LONG_DOUBLE == 1 */

xtnt_uint_t xtnt_hash(void *key);

#endif /* ifndef _XTNT_COMMON_H */
