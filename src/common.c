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

#include <extant/extant.h>

/**
 * @brief Generate a hash from a key
 *
 * Generate a unique integer evaluating the bytes of a key. There is collision
 * but for null terminated string keys < 128 bytes long this works well.
 *
 * @param[in] key Value to be hashed
 * @return xtnt_uint_t hash value
 */
xtnt_uint_t xtnt_hash(void *key)
{
    const char *s = (const char*) key;
    xtnt_uint_t sum = 0;
    for (xtnt_int_t idx = 0; s[idx] != 0; idx++) {
        if !(idx & 1) {
            sum <<= sizeof(char);
        }
        sum ^= (xtnt_int_t) s[idx];
    }
    return sum;
}
