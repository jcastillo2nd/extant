/*
===============================================================================

This source file is part of libextant
libextant is made available under the MIT License.

Copyright (c) 2016-2020 Javier Castillo II

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
#include <extant/set/node.h>

#include <stdio.h>

struct xtnt_node node1;
struct xtnt_node node2;
xtnt_uint_t value1 = 1;
xtnt_uint_t value2 = 2;

void setup(void)
{
}

void teardown(void)
{
}

START_TEST (test_xtnt_node_initialize)
{
    struct xtnt_node *res = &node1;
    xtnt_status_t test = XTNT_EFAILURE;
    test = xtnt_node_initialize(res, 5, 34, &value1);
    ck_assert_msg(test == XTNT_ESUCCESS,
        "Initialization failed with %d", test);
    ck_assert_msg(res->key == 5,
        "Expected node of key 5 but got %u", res->key);
    ck_assert_msg(res->value == &value1,
        "Expected node value of 1, but got %u", (xtnt_uint_t *) res->value);
    ck_assert_msg(res->quirk == 34,
        "Expected quirk value of %d, but got %d", 34, res->quirk);
    ck_assert_msg(res->link[XTNT_NODE_HEAD] == NULL,
        "Expected node head as NULL, but was not NULL");
    ck_assert_msg(res->link[XTNT_NODE_MIDDLE] == NULL,
        "Expected node middle as NULL, but was not NULL");
    ck_assert_msg(res->link[XTNT_NODE_TAIL] == NULL,
        "Expected node tail as NULL, but was not NULL");
}

START_TEST (test_xtnt_node_uninitialize)
{
    struct xtnt_node *node = &node2;
    xtnt_status_t res = xtnt_node_uninitialize(node);
    ck_assert_msg(res == XTNT_ESUCCESS,
        "Uninitialization failed with %d", res);
}
END_TEST

Suite * xtnt_set_common_suite(void)
{
    Suite *s;
    TCase *tc_set_common;

    s = suite_create("xtnt_set");

    tc_set_common = tcase_create("Set Common");

    tcase_add_checked_fixture(tc_set_common, setup, teardown);
    tcase_add_test(tc_set_common, test_xtnt_node_initialize);
    tcase_add_test(tc_set_common, test_xtnt_node_uninitialize);
    suite_add_tcase(s, tc_set_common);

    return s;
}

int main(void)
{
    int failed;
    Suite *s;
    SRunner *sr;

    s = xtnt_set_common_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? 0 : failed;
}
