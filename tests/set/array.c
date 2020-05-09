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

void setup(void)
{
    struct xtnt_node nodes[8];
    for (xtnt_int_t idx = 0; idx < 8; idx++) {
        nodes[idx].value = NULL;
        nodes[idx].link[0] = (idx == 0)? NULL : &nodes[idx - 1];
        nodes[idx].link[2] = (idx == 7)? &nodes[idx + 1]: NULL;
        nodes[idx].key = idx;
        nodes[idx].quirk = XTNT_ESUCCESS;
        if (pthread_mutex_init(&(nodes[idx].lock), NULL)) {
            XTNT_LOCK_SET_INIT_FAIL(nodes[idx].state);
        } else {
            XTNT_LOCK_SET_VALUE(nodes[idx].state, XTNT_LOCK_STATE_SUCCESS);
        }
    }
}

void teardown(void)
{
}

START_TEST (test_xtnt_array_create)
{
    struct xtnt_node_set *array;
    xtnt_status_t status;
    status = xtnt_array_create(8, &array);
    if (status == XTNT_ESUCCESS) {
        ck_assert_msg(array != NULL,
            "Expected array as valid pointer, but got NULL");
        ck_assert_msg(array->count == 8,
            "Expected array size of 8, but got %d", array->count);

        ck_assert_msg(array->link[XTNT_NODE_HEAD] != NULL,
            "Expected link head with valid memory pointer, but got NULL");

        ck_assert_msg(array->link[XTNT_NODE_TAIL] != NULL,
            "Expected link tail with valid memory pointer, but got NULL");

        ck_assert_msg(array->link[XTNT_NODE_TAIL] == &(array->link[XTNT_NODE_HEAD][7]),
            "Expected link tail set to final node in array of size 8");
    } else {
        ck_assert_msg(status == XTNT_EFAILURE,
            "Memory allocation failed. Test not completed");
        ck_assert_msg(XTNT_LOCK(array->state),
            "Node set mutex lock failure. Mutex operation returned %d", status);
    }
}
END_TEST

START_TEST (test_xtnt_array_destroy)
{
    struct xtnt_node_set *array;
    xtnt_status_t status;
    status = xtnt_array_create(8, &array);
    if (status == XTNT_ESUCCESS) {
        if ((status = xtnt_array_destroy(&array)) != XTNT_ESUCCESS) {
            ck_assert_msg(array != NULL,
                "Expected non-NULL array pointer after failed destroy attempt");
        } else {
            ck_assert_msg(array == NULL,
                "Expected NULL array pointer after successful destroy");
        }
    } else {
        ck_assert_msg(status == XTNT_EFAILURE,
            "Memory allocation failed. Test not completed");
        ck_assert_msg(XTNT_LOCK(array->state),
            "Node set mutex lock failure. Mutex operation returned %d", status);
    }
}
END_TEST

START_TEST (test_xtnt_array_delete)
{
}
END_TEST

Suite * xtnt_list_suite(void)
{
    Suite *s;
    TCase *tc_xtnt_list;

    s = suite_create("xtnt_array_tests");

    tc_xtnt_list = tcase_create("Array tests");
    tcase_add_checked_fixture(tc_xtnt_list, setup, teardown);

    tcase_add_test(tc_xtnt_list, test_xtnt_array_create);
    tcase_add_test(tc_xtnt_list, test_xtnt_array_destroy);
    suite_add_tcase(s, tc_xtnt_list);

    return s;
}

int main(void)
{
    int failed;
    Suite *s;
    SRunner *sr;

    s = xtnt_list_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? 0 : failed;
}
