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
#include <extant/dso.h>

#include <stdio.h>

struct xtnt_dso *dso; 

void setup(void)
{
}

void teardown(void)
{
}

START_TEST (test_xtnt_dso)
{
}
END_TEST

Suite * xtnt_dso_suite(void)
{
    Suite *s;
    TCase *tc_dso;

    s = suite_create("xtnt_dso");

    tc_dso = tcase_create("DSO");

    tcase_add_checked_fixture(tc_dso, setup, teardown);
    tcase_add_test(tc_dso, test_xtnt_dso);
    suite_add_tcase(s, tc_dso);

    return s;
}

int main(void)
{
    int failed;
    Suite *s;
    SRunner *sr;

    s = xtnt_dso_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? 0 : failed;
}
