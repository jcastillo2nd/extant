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
#include <stdio.h>

int xtnt_node_initialize_tests();

int main(int argc, char** argv)
{
    return (
            xtnt_node_initialize_tests()
   );
}

int xtnt_node_initialize_tests()
{
    int i;
    struct xtnt_node node1;
    struct xtnt_node node2;
    xtnt_node_initialize(&node1, 80, NULL);
    xtnt_node_initialize(&node2, 44, &i);
    // Check Node key is properly set
    if (node1.key != 80) {
        return 1;
    }
    // Check node NULL value is NULL
    if (node1.value != NULL) {
        return 2;
    }
    // Check node value is properly set
    if (node2.value != &1) {
	return 3;
    }
    // Check node1 links are NULL
    if (node1.link[0] != NULL || \
        node1.link[1] != NULL || \
	node1.link[2] != NULL) {
	return 4;
    }
    // Check node2 links are NULL
    if (node1.link[0] != NULL || \
        node1.link[1] != NULL || \
	node1.link[2] != NULL) {
	return 4;
    }
    return 0;
}
