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

#include <check.h>
#include <extant/set/stack.h>

#include <stdio.h>

struct xtnt_node node1t1, node2t1, node3t1, node1t2, node2t2, node3t2;
struct xtnt_node_set tree1, tree2;
xtnt_uint_t value1, value2, value3;

void setup(void)
{
    node1t1.key = 1;
    node1t1.value = &value1;
    node1t1.link[XTNT_NODE_HEAD] = &node2t1;
    node1t1.link[XTNT_NODE_MIDDLE] = \
    node1t1.link[XTNT_NODE_TAIL] = NULL;

    node2t1.key = 2;
    node2t1.value = &value2;
    node2t1.link[XTNT_NODE_HEAD] = &node3t1;
    node2t1.link[XTNT_NODE_MIDDLE] = NULL;
    node2t1.link[XTNT_NODE_TAIL] = &node1t1;

    node3t1.key = 3;
    node3t1.value = &value3;
    node3t1.link[XTNT_NODE_HEAD] = \
    node3t1.link[XTNT_NODE_MIDDLE] = NULL;
    node3t1.link[XTNT_NODE_TAIL] = &node2t1;

    tree1.link[XTNT_NODE_HEAD] = &node3t1;
    tree1.link[XTNT_NODE_MIDDLE] = NULL;
    tree1.link[XTNT_NODE_TAIL] = &node1t1;
    tree1.count = 3;

    node1t2.key = 4;
    node1t2.value = &value1;
    node1t2.link[0] = node1t2.link[1] = node1t2.link[2] = NULL;

    node2t2.key = 5;
    node2t2.value = &value2;
    node2t2.link[0] = node2t2.link[1] = node2t2.link[2] = NULL;

    node3t2.key = 6;
    node3t2.value = &value3;
    node3t2.link[0] = node3t2.link[1] = node3t2.link[2] = NULL;

    tree2.link[XTNT_NODE_HEAD] = NULL;
    tree2.link[XTNT_NODE_MIDDLE] = NULL;
    tree2.link[XTNT_NODE_TAIL] = NULL;
    tree2.count = 0;
}

void teardown(void)
{
}

START_TEST (test_xtnt_avltree_simple_test)
{
}
END_TEST

Suite * xtnt_avltree_suite(void)
{
    Suite *s;
    TCase *tc_xtnt_stack;

    s = suite_create("xtnt_stack");

    tc_xtnt_stack = tcase_create("Stack");

    tcase_add_checked_fixture(tc_xtnt_stack, setup, teardown);
    tcase_add_test(tc_xtnt_stack, test_xtnt_avltree_simple_test);
    suite_add_tcase(s, tc_xtnt_stack);

    return s;
}

int main(void)
{
    int failed;
    Suite *s;
    SRunner *sr;

    s = xtnt_avltree_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? 0 : failed;
}
