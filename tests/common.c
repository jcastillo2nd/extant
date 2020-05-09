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
#include <extant/extant.h>
#include <stdio.h>

START_TEST (test_XTNT_ZERO_returns_0)
{
    ck_assert_msg(XTNT_ZERO == 0,
        "XTNT_ZERO is not 0");
}
END_TEST

START_TEST (test_XTNT_HASH_CMP_results_are_correct)
{
    ck_assert_msg(XTNT_HASH_CMP(1,2) == -1,
        "XTNT_HASH_CMP(1,2) did not return -1");
    ck_assert_msg(XTNT_HASH_CMP(2,2) == 0,
        "XTNT_HASH_CMP(2,2) did not return 0");
    ck_assert_msg(XTNT_HASH_CMP(3,2) == 1,
        "XTNT_HASH_CMP(3,2) did not return 1");
}
END_TEST

START_TEST (test_XTNT_STATE_results_are_correct)
{
    ck_assert_msg(XTNT_STATE(64) == 1,
        "XTNT_STATE(64) did not return 1, instead returned %d", XTNT_STATE(64));
    ck_assert_msg(XTNT_STATE(128) == 2,
        "XTNT_STATE(128) did not return 2, instead returned %d", XTNT_STATE(128));
    ck_assert_msg(XTNT_STATE(192) == 3,
        "XTNT_STATE(192) did not return 3, instead returned %d", XTNT_STATE(192));
}
END_TEST

START_TEST (test_XTNT_STATE_VALUE_results_are_correct)
{
    int state = 1;
    ck_assert_msg(XTNT_STATE_VALUE(state, 1) == 65,
        "XTNT_STATE_VALUE(1, 1) did not return 65, instead returned %d", XTNT_STATE_VALUE(state, 1));
    ck_assert_msg(XTNT_STATE_VALUE(state, 2) == 129,
        "XTNT_STATE_VALUE(1, 2) did not return 129, instead returned %d", XTNT_STATE_VALUE(state, 2));
    ck_assert_msg(XTNT_STATE_VALUE(state, 3) == 193,
        "XTNT_STATE_VALUE(1, 3) did not return 193, instead returned %d", XTNT_STATE_VALUE(state, 3));
}
END_TEST

START_TEST (test_XTNT_STATE_SET_VALUE_sets_correctly)
{
    int state = 1;
    XTNT_STATE_SET_VALUE(state, 1);
    ck_assert_msg(state == 65,
        "XTNT_STATE_VALUE(1, 1) did not set 65, instead set %d", state);
    XTNT_STATE_SET_VALUE(state, 2);
    ck_assert_msg(state == 129,
        "XTNT_STATE_VALUE(1, 2) did not set 129, instead set %d", state);
    XTNT_STATE_SET_VALUE(state, 3);
    ck_assert_msg(state == 193,
        "XTNT_STATE_VALUE(1, 3) did not set 193, instead set %d", state);
}
END_TEST

START_TEST (test_XTNT_MODE_results_are_correct)
{
    ck_assert_msg(XTNT_MODE(64) == 0,
        "XTNT_MODE(64) did not return 0, instead returned %d", XTNT_MODE(64));
    ck_assert_msg(XTNT_MODE(65) == 1,
        "XTNT_MODE(65) did not return 1, instead returned %d", XTNT_MODE(65));
    ck_assert_msg(XTNT_MODE(66) == 2,
        "XTNT_MODE(66) did not return 2, instead returned %d", XTNT_MODE(66));
    ck_assert_msg(XTNT_MODE(67) == 3,
        "XTNT_MODE(67) did not return 3, instead returned %d", XTNT_MODE(67));
}
END_TEST

START_TEST (test_XTNT_MODE_VALUE_results_are_correct)
{
    ck_assert_msg(XTNT_MODE_VALUE(64, 1) == 65,
        "XTNT_MODE_VALUE(64, 1) did not return 65, instead returned %d", XTNT_MODE_VALUE(64, 1));
    ck_assert_msg(XTNT_MODE_VALUE(64, 2) == 66,
        "XTNT_MODE_VALUE(64, 2) did not return 66, instead returned %d", XTNT_MODE_VALUE(64, 2));
    ck_assert_msg(XTNT_MODE_VALUE(64, 3) == 67,
        "XTNT_MODE_VALUE(64, 3) did not return 67, instead returned %d", XTNT_MODE_VALUE(64, 3));
}
END_TEST

START_TEST (test_XTNT_MODE_SET_VALUE_sets_correctly)
{
    int state = 64;
    XTNT_MODE_SET_VALUE(state, 1);
    ck_assert_msg(state == 65,
        "XTNT_MODE_SET_VALUE(64, 1) did not set 65, instead set %d", state);
    XTNT_MODE_SET_VALUE(state, 2);
    ck_assert_msg(state == 66,
        "XTNT_MODE_SET_VALUE(64, 2) did not set 66, instead set %d", state);
    XTNT_MODE_SET_VALUE(state, 3);
    ck_assert_msg(state == 67,
        "XTNT_MODE_SET_VALUE(64, 3) did not set 67, instead set %d", state);
}
END_TEST

START_TEST (test_XTNT_LOCK_results_are_correct)
{
    ck_assert_msg(XTNT_LOCK(68) == 4,
        "XTNT_LOCK(68) did not return 4, instead returned %d", XTNT_LOCK(68));
    ck_assert_msg(XTNT_LOCK(76) == 12,
        "XTNT_LOCK(76) did not return 12, instead returned %d", XTNT_LOCK(76));
    ck_assert_msg(XTNT_LOCK(92) == 28,
        "XTNT_LOCK(92) did not return 28, instead returned %d", XTNT_LOCK(92));
    ck_assert_msg(XTNT_LOCK(124) == 60,
        "XTNT_LOCK(124) did not return 60, instead returned %d", XTNT_LOCK(124));
}
END_TEST

START_TEST (test_XTNT_LOCK_VALUE_results_are_correct)
{
    ck_assert_msg(XTNT_LOCK_VALUE(64, 4) == 68,
        "XTNT_LOCK(64, 4) did not return 68, instead returned %d", XTNT_LOCK_VALUE(64, 4));
    ck_assert_msg(XTNT_LOCK_VALUE(64, 12) == 76,
        "XTNT_LOCK(64, 12) did not return 76, instead returned %d", XTNT_LOCK_VALUE(64, 12));
    ck_assert_msg(XTNT_LOCK_VALUE(64, 28) == 92,
        "XTNT_LOCK(64, 28) did not return 92, instead returned %d", XTNT_LOCK_VALUE(64, 28));
    ck_assert_msg(XTNT_LOCK_VALUE(64, 60) == 124,
        "XTNT_LOCK(64, 60) did not return 124, instead returned %d", XTNT_LOCK_VALUE(64, 60));
}
END_TEST

START_TEST (test_XTNT_LOCK_SET_VALUE_sets_correctly)
{
    int state = 64;
    XTNT_LOCK_SET_VALUE(state, 4);
    ck_assert_msg(state == 68,
        "XTNT_LOCK_SET_VALUE(64, 4) did not set 68, instead set %d", state);
    XTNT_LOCK_SET_VALUE(state, 12);
    ck_assert_msg(state == 76,
        "XTNT_LOCK_SET_VALUE(64, 12) did not set 76, instead set %d", state);
    XTNT_LOCK_SET_VALUE(state, 28);
    ck_assert_msg(state == 92,
        "XTNT_LOCK_SET_VALUE(64, 28) did not set 92, instead set %d", state);
    XTNT_LOCK_SET_VALUE(state, 60);
    ck_assert_msg(state == 124,
        "XTNT_LOCK_SET_VALUE(64, 60) did not set 124, instead set %d", state);
}
END_TEST

START_TEST (test_xtnt_hash_returns_3817775)
{
  char name[] = "This is a string";
  xtnt_uint_t hash = xtnt_hash(name);
  ck_assert_msg(hash == 3817775,
		"expecting 3817775 but got %u instead", hash);
}
END_TEST

Suite * xtnt_common_suite(void)
{
    Suite *s;
    TCase *tc_common;

    s = suite_create("xtnt_common");

    tc_common = tcase_create("Common");

    tcase_add_test(tc_common, test_XTNT_ZERO_returns_0);
    tcase_add_test(tc_common, test_XTNT_HASH_CMP_results_are_correct);
    tcase_add_test(tc_common, test_XTNT_STATE_results_are_correct);
    tcase_add_test(tc_common, test_XTNT_STATE_VALUE_results_are_correct);
    tcase_add_test(tc_common, test_XTNT_STATE_SET_VALUE_sets_correctly);
    tcase_add_test(tc_common, test_XTNT_MODE_results_are_correct);
    tcase_add_test(tc_common, test_XTNT_MODE_VALUE_results_are_correct);
    tcase_add_test(tc_common, test_XTNT_LOCK_results_are_correct);
    tcase_add_test(tc_common, test_XTNT_LOCK_VALUE_results_are_correct);
    tcase_add_test(tc_common, test_xtnt_hash_returns_3817775);
    suite_add_tcase(s, tc_common);

    return s;
}

int main(void)
{
    int failed;
    Suite *s;
    SRunner *sr;

    s = xtnt_common_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? 0 : failed;
}
