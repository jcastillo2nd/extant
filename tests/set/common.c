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
#include <extant/set/common.h>

#include <stdio.h>

struct xtnt_node_set set1;
struct xtnt_node_set set2;
xtnt_uint_t value;

void setup(void)
{
}

void teardown(void)
{
}

START_TEST (test_xtnt_set_initialize)
{
    struct xtnt_node_set *set = &set1;
    xtnt_int_t test = xtnt_node_set_initialize(set);
    ck_assert_msg(test == 0,
        "Initialization failed with %d", test);
    ck_assert_msg(set->root.link[XTNT_NODE_HEAD] == NULL,
        "Expected set head as NULL, but was not NULL");
    ck_assert_msg(set->root.link[XTNT_NODE_MIDDLE] == NULL,
        "Expected set middle as NULL, but was not NULL");
    ck_assert_msg(set->root.link[XTNT_NODE_TAIL] == NULL,
        "Expected set tail as NULL, but was not NULL");
    ck_assert_msg(set->count == 0,
        "Expected set count as 0, but got %u", set->count);
    ck_assert_msg(set->root.state == 0,
        "Expected set state as 0, but got %u", set->root.state);
}
END_TEST

START_TEST (test_xtnt_set_uninitialize)
{
    struct xtnt_node_set *set = &set2;
    xtnt_status_t res = xtnt_node_set_uninitialize(set);
    ck_assert_msg(res == XTNT_ESUCCESS,
        "Expected xtnt_node_set_uninitialize to succeed");
}
END_TEST

Suite * xtnt_set_common_suite(void)
{
    Suite *s;
    TCase *tc_set_common;

    s = suite_create("xtnt_set");

    tc_set_common = tcase_create("Set Common");

    tcase_add_checked_fixture(tc_set_common, setup, teardown);
    tcase_add_test(tc_set_common, test_xtnt_set_initialize);
    tcase_add_test(tc_set_common, test_xtnt_set_uninitialize);
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
