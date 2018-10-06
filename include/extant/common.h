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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#ifndef _XTNT_COMMON_H_
#define _XTNT_COMMON_H_

#include <errno.h>
#include <sched.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <pthread.h>

/**
 * @def XTNT_ZERO
 * Constant 0 reference
 */
#define XTNT_ZERO (0)

/**
 * @def XTNT_HASH_CMP(X, Y)
 * Set -1 if less than, 0 if equal, and 1 if greater than
 */
#define XTNT_HASH_CMP(X, Y) ((X<Y)?-1:(X>Y))

/**
 * @def XTNT_STATE_MASK
 * Six bit state mask
 * [LOCK BIT 1][LOCK BIT 0][MODE BIT 1][MODE BIT 0]
 */
#define XTNT_STATE_MASK (63)
#define XTNT_STATE_MASK_SHIFT (6)

/**
 * @def XTNT_STATE(S)
 * Evaluate state S
 *
 * @warning The state value has a limit of XTNT_UINT_MAX or XTNT_INT_MAX minus
 * 6 bits. E.g. on a 32 bit int, the maximum unsigned value would be 2^26 or 
 * unsigned 67108864, and signed range from -33554433 - 33554432.
 */
#define XTNT_STATE(S) (S >> XTNT_STATE_MASK_SHIFT)

/**
 * @def XTNT_STATE_VALUE(S, V)
 * Evaluate state S with new value V
 */
#define XTNT_STATE_VALUE(S, V) ((V << XTNT_STATE_MASK_SHIFT) | (S & XTNT_STATE_MASK))

/**
 * @def XTNT_STATE_SET_VALUE(S, V)
 * Assign state S with new value V
 */
#define XTNT_STATE_SET_VALUE(S, V) S = XTNT_STATE_VALUE(S, V)

#define XTNT_LOCK_DESTROY_FAIL (64)

/**
 * @def XTNT_MODE_MASK
 * Two bit mode mask for state
 */
#define XTNT_MODE_MASK (3)

#define XTNT_MODE_1 (0) /**< Mode 1 value */
#define XTNT_MODE_2 (1) /**< Mode 2 value */
#define XTNT_MODE_3 (2) /**< Mode 3 value */
#define XTNT_MODE_4 (3) /**< Mode 4 value */

/**
 * @def XTNT_MODE(S)
 * Evaluate mode for state S
 */
#define XTNT_MODE(S) (S & XTNT_MODE_MASK)

/**
 * @def XTNT_MODE_VALUE(S, M)
 * Evalute state S value with new mode M 
 */
#define XTNT_MODE_VALUE(S, M) ((S & ~XTNT_MODE_MASK) | (M & XTNT_MODE_MASK))

/**
 * @def XTNT_MODE_SET_VALUE(S, M)
 * Assign state S with new mode M
 */
#define XTNT_MODE_SET_VALUE(S, M) S = XTNT_MODE_VALUE(S, M)

/**
 * @def XTNT_LOCK_MASK
 * Four bit lock failure status [destroy][init][lock][fail]
 */
#define XTNT_LOCK_MASK (60)

#define XTNT_LOCK_STATE_SUCCESS (0) /**< Successful lock status */
#define XTNT_LOCK_STATE_FAIL_LOCK (4) /**< Failed to lock */
#define XTNT_LOCK_STATE_FAIL_UNLOCK (12) /**< Failed to unlock */
#define XTNT_LOCK_STATE_FAIL_INIT (28) /**< Failed to init */
#define XTNT_LOCK_STATE_FAIL_DESTROY (60) /**< Failed to destroy */

/**
 * @def XTNT_LOCK(L)
 * Evaluate lock status for state S
 */
#define XTNT_LOCK(S) (S & XTNT_LOCK_MASK)

/**
 * @def XTNT_LOCK_VALUE(S, L)
 * Evalute state S with new lock status L
 */
#define XTNT_LOCK_VALUE(S, L) ((S & ~XTNT_LOCK_MASK) | (L & XTNT_LOCK_MASK))

/**
 * @def XTNT_LOCK_SET_VALUE(S, L)
 * Assign state S with new lock status L
 */
#define XTNT_LOCK_SET_VALUE(S, L) S = XTNT_LOCK_VALUE(S, L)

/**
 * @def XTNT_LOCK_SET_LOCK_FAIL(S)
 * Assign state S with lock failure status
 */
#define XTNT_LOCK_SET_LOCK_FAIL(S) XTNT_LOCK_SET_VALUE(S, XTNT_LOCK_STATE_FAIL_LOCK)

/**
 * @def XTNT_LOCK_SET_UNLOCK_FAIL(S)
 * Assign state S with unlock failure status
 */
#define XTNT_LOCK_SET_UNLOCK_FAIL(S) XTNT_LOCK_SET_VALUE(S, XTNT_LOCK_STATE_FAIL_UNLOCK)

/**
 * @def XTNT_LOCK_SET_INIT_FAIL(S)
 * Assign state S with lock init failure status
 */
#define XTNT_LOCK_SET_INIT_FAIL(S) XTNT_LOCK_SET_VALUE(S, XTNT_LOCK_STATE_FAIL_INIT)

/**
 * @def XTNT_LOCK_SET_DESTROY_FAIL(S)
 * Assign state S with lock destroy failure status
 */
#define XTNT_LOCK_SET_DESTROY_FAIL(S) XTNT_LOCK_SET_VALUE(S, XTNT_LOCK_STATE_FAIL_DESTROY)

#if HAVE_LONG_LONG_INT == 1
#define XTNT_INT_MAX LLONG_MAX
typedef long long int xtnt_int_t;
/* #endif HAVE_LONG_LONG_INT */
#elif HAVE_LONG_INT == 1
/* #if HAVE_LONG_INT == 1 */
#define XTNT_INT_MAX LONG_MAX
typedef long int xtnt_int_t;
/* #endif HAVE_LONG_INT == 1 */
#else
/* #if HAVE_INT == 1 */
#define XTNT_INT_MAX INT_MAX
typedef int xtnt_int_t;
/* #endif HAVE_INT == 1 */
#endif 

#if HAVE_UNSIGNED_LONG_LONG_INT == 1
#define XTNT_UINT_MAX ULLONG_MAX
typedef unsigned long long int xtnt_uint_t;
/* #endif HAVE_UNSIGNED_LONG_LONG_INT */
#elif HAVE_UNSIGNED_LONG_INT == 1
/* #if HAVE_UNSIGNED_LONG_INT == 1 */
#define XTNT_UINT_MAX ULONG_MAX
typedef unsigned long int xtnt_uint_t;
/* #endif HAVE_UNSIGNED_LONG_INT == 1 */
#else
/* #if HAVE_UNSIGNED_INT == 1 */
#define XTNT_UINT_MAX UINT_MAX
typedef unsigned int xtnt_uint_t;
/* #endif HAVE_UNSIGNED_INT == 1 */
#endif 

#if HAVE_LONG_DOUBLE == 1
typedef long double xtnt_real_t;
/* #endif HAVE_LONG_DOUBLE == 1 */
#elif HAVE_DOUBLE == 1
/* #if HAVE_DOUBLE == 1 */
typedef double xtnt_real_t;
/* #endif HAVE_DOUBLE == 1 */
#else
/* #if HAVE_FLOAT == 1 */
typedef float xtnt_real_t;
/* #endif HAVE_FLOAT == 1 */
#endif

typedef xtnt_real_t xtnt_radian_t;
typedef xtnt_real_t xtnt_degree_t;

xtnt_uint_t
xtnt_hash(
    void *key);

#define xtnt_test(S) S

#endif /* ifndef _XTNT_COMMON_H */
