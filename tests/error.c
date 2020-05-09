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

#define XTNT_IS_EFAILURE(S) (S < XTNT_ZERO)
#define XTNT_IS_ESUCCESS(S) (S == XTNT_ZERO)
#define XTNT_IS_EWARNING(S) (S > XTNT_ZERO)

START_TEST (test_XTNT_IS_EFAILURE_results_are_correct)
{
    ck_assert_msg(XTNT_IS_EFAILURE(-1),
        "Expected XTNT_IS_EFAILURE(-1) to be true");
    ck_assert_msg(!XTNT_IS_EFAILURE(0),
        "Expected XTNT_IS_EFAILURE(0) to be false");
    ck_assert_msg(!XTNT_IS_EFAILURE(1),
        "Expected XTNT_IS_EFAILURE(1) to be false");
}
END_TEST

START_TEST (test_XTNT_IS_ESUCCESS_results_are_correct)
{
    ck_assert_msg(!XTNT_IS_ESUCCESS(-1),
        "Expected XTNT_IS_ESUCCESS(-1) to be false");
    ck_assert_msg(XTNT_IS_ESUCCESS(0),
        "Expected XTNT_IS_ESUCCESS(0) to be true");
    ck_assert_msg(!XTNT_IS_ESUCCESS(1),
        "Expected XTNT_IS_ESUCCESS(1) to be false");
}
END_TEST

START_TEST (test_XTNT_IS_EWARNING_results_are_correct)
{
    ck_assert_msg(!XTNT_IS_EWARNING(-1),
        "Expected XTNT_IS_EWARNING(-1) to be false");
    ck_assert_msg(!XTNT_IS_EWARNING(0),
        "Expected XTNT_IS_EWARNING(0) to be false");
    ck_assert_msg(XTNT_IS_EWARNING(1),
        "Expected XTNT_IS_EWARNING(1) to be true");
}
END_TEST

Suite * xtnt_error_suite(void)
{
    Suite *s;
    TCase *tc_error;

    s = suite_create("xtnt_status");

    tc_error = tcase_create("Error");

    tcase_add_test(tc_error, test_XTNT_IS_EFAILURE_results_are_correct);
    tcase_add_test(tc_error, test_XTNT_IS_ESUCCESS_results_are_correct);
    tcase_add_test(tc_error, test_XTNT_IS_EWARNING_results_are_correct);
    suite_add_tcase(s, tc_error);

    return s;
}

int main(void)
{
    int failed;
    Suite *s;
    SRunner *sr;

    s = xtnt_error_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? 0 : failed;
}
