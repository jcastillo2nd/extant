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

#include <cgre/cgre.h>
#include <stdio.h>

int cgre_tree_replace_tests();

int main(int argc, char** argv)
{
    return (
        cgre_tree_replace_tests()
   );
}

int cgre_tree_replace_tests()
{
    struct cgre_node_set tree1;
    struct cgre_node root1;
    struct cgre_node new1;
    struct cgre_node new2;
    cgre_node_initialize(&root1, 44, NULL);
    cgre_node_initialize(&new1, 99, &new1);
    cgre_node_initialize(&new2, 99, &new2);
    cgre_node_set_initialize(&tree1);
    cgre_tree_insert(&tree1, &root1);
    cgre_tree_insert(&tree1, &new1);
    if (cgre_tree_replace(&tree1, &new2) != &new1) {
        return 1;
    }
    if (tree1.count != 2) {
        return 2;
    }
    return 0;
}
