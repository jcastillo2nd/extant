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
#include <extant/set/list.h>

#include <stdio.h>

struct xtnt_node_set sets[2];
struct xtnt_node nodes[16];
struct xtnt_node in_nodes[16];
xtnt_uint_t values[16];

void setup(void)
{
    for (int idx=0; idx<16; idx++){
        nodes[idx].key = idx;
        values[idx] = idx;
        nodes[idx].value = &values[idx];
        if (idx != 0){
            nodes[idx].link[XTNT_NODE_HEAD]=&nodes[idx-1];
        } else {
            nodes[idx].link[XTNT_NODE_HEAD]=NULL;
        }
        nodes[idx].link[XTNT_NODE_MIDDLE] = NULL;
        if (idx != 15){
            nodes[idx].link[XTNT_NODE_TAIL]=&nodes[idx+1];
        } else {
            nodes[idx].link[XTNT_NODE_TAIL]=NULL;
        }
    }

    for (int idx=0; idx<16; idx++){
        in_nodes[idx].key = idx + 16;
        in_nodes[idx].value = &values[idx];
        in_nodes[idx].link[XTNT_NODE_HEAD] = \
        in_nodes[idx].link[XTNT_NODE_MIDDLE] = \
        in_nodes[idx].link[XTNT_NODE_HEAD] = NULL;
    }

    sets[0].link[XTNT_NODE_HEAD] = &nodes[0];
    sets[0].link[XTNT_NODE_MIDDLE] = NULL;
    sets[0].link[XTNT_NODE_TAIL] = &nodes[15];
    sets[0].count = 16;

    sets[1].link[XTNT_NODE_HEAD] = \
    sets[1].link[XTNT_NODE_MIDDLE] = \
    sets[1].link[XTNT_NODE_TAIL] = NULL;
    sets[1].count = 0;

    if (pthread_mutex_init(&sets[0].lock, NULL)) {
        ck_abort_msg("Failed to initialize set0.lock in setup.");
    }

    if (pthread_mutex_init(&sets[1].lock, NULL)) {
        ck_abort_msg("Failed to initialize set1.lock in setup.");
    }
}

void teardown(void)
{
    pthread_mutex_destroy(&sets[0].lock);
}

START_TEST (test_xtnt_list_delete_empty)
{
    for (xtnt_uint_t idx = 0; idx < 16; idx++) {
        struct xtnt_node *deleted = xtnt_list_delete(&sets[1], idx);
        ck_assert_msg(deleted == NULL,
            "Expected deleted to be NULL but got node%u", deleted->key);
    }
}
END_TEST

START_TEST (test_xtnt_list_delete_up)
{
    for (xtnt_uint_t idx = 0; idx < 16; idx++) {
        struct xtnt_node *deleted = xtnt_list_delete(&sets[0], 0);
        ck_assert_msg(deleted == &nodes[idx],
            "Expected deleted as node%u but got node%u", idx, deleted->key);
        ck_assert_msg(deleted->key == idx,
            "Expected node%u with key %u, but got node.key=%u", idx, idx, deleted->key);
        ck_assert_msg(sets[0].count == (16 - (idx + 1)),
            "Expected set.count of %u, but got set.count=%u", (16 - (idx + 1)), sets[0].count);
        if (idx < 15) {
            ck_assert_msg(sets[0].link[XTNT_NODE_HEAD] == &nodes[idx + 1],
                "Expected set head as node%u but got node%u", (idx + 1), sets[0].link[XTNT_NODE_HEAD]->key);
            ck_assert_msg(sets[0].link[XTNT_NODE_TAIL] == &nodes[15],
                "Expected set tail as node0 but was not node0");
        } else {
            ck_assert_msg(sets[0].link[XTNT_NODE_HEAD] == NULL,
                "Expected set head as NULL but got node%u", sets[0].link[XTNT_NODE_HEAD]->key);
            ck_assert_msg(sets[0].link[XTNT_NODE_TAIL] == NULL,
                "Expected set tail as NULL but got node%u", sets[0].link[XTNT_NODE_TAIL]->key);
        }
    }
}
END_TEST

START_TEST (test_xtnt_list_delete_down)
{
    for (xtnt_uint_t idx = 16; idx > 0; idx--) {
        xtnt_uint_t cidx = idx - 1;
        struct xtnt_node *deleted = xtnt_list_delete(&sets[0], cidx);
        ck_assert_msg(deleted == &nodes[cidx],
            "Expected deleted as node%u but got node%u", cidx, deleted->key);
        ck_assert_msg(deleted->key == cidx,
            "Expected node%u with key %u, but got node.key=%u", cidx, cidx, deleted->key);
        ck_assert_msg(sets[0].count == cidx,
            "Expected set.count of %u, but got set.count=%u", cidx, sets[0].count);
        if (idx > 1) {
            ck_assert_msg(sets[0].link[XTNT_NODE_HEAD] == &nodes[0],
                "Expected set head as node0 but was not node0");
            ck_assert_msg(sets[0].link[XTNT_NODE_TAIL] == &nodes[cidx - 1],
                "Expected set head as node%u but got node%u", (cidx - 1), sets[0].link[XTNT_NODE_TAIL]->key);
        } else {
            ck_assert_msg(sets[0].link[XTNT_NODE_HEAD] == NULL,
                "Expected set head as NULL but got node%u", sets[0].link[XTNT_NODE_HEAD]->key);
            ck_assert_msg(sets[0].link[XTNT_NODE_TAIL] == NULL,
                "Expected set tail as NULL but got node%u", sets[0].link[XTNT_NODE_TAIL]->key);
        }
    }
}
END_TEST

START_TEST (test_xtnt_list_delete_node7)
{
    struct xtnt_node *deleted = xtnt_list_delete(&sets[0], 7);
    ck_assert_msg(deleted == &nodes[7],
        "Expected node7 but was not node7");
    ck_assert_msg(deleted->key == 7,
        "Expected node7 with key 7, but got node.key=%u", deleted->key);
    ck_assert_msg(sets[0].count == 15,
        "Expected set.count of 15, but got set.count=%u", sets[0].count);
    ck_assert_msg(sets[0].link[XTNT_NODE_HEAD] == &nodes[0],
        "Expected set head as node0 but was not node0");
    ck_assert_msg(sets[0].link[XTNT_NODE_TAIL] == &nodes[15],
        "Expected set tail as node15 but was not node15");
}
END_TEST

START_TEST (test_xtnt_list_delete_node8)
{
    struct xtnt_node *deleted = xtnt_list_delete(&sets[0], 8);
    ck_assert_msg(deleted == &nodes[8],
        "Expected node8 but was not node8");
    ck_assert_msg(deleted->key == 8,
        "Expected node8 with key 8, but got node.key=%u", deleted->key);
    ck_assert_msg(sets[0].count == 15,
        "Expected set.count of 15, but got set.count=%u", sets[0].count);
    ck_assert_msg(sets[0].link[XTNT_NODE_HEAD] == &nodes[0],
        "Expected set head as node0 but was not node0");
    ck_assert_msg(sets[0].link[XTNT_NODE_TAIL] == &nodes[15],
        "Expected set tail as node15 but was not node15");
}
END_TEST

START_TEST (test_xtnt_list_insert_full)
{
    for (xtnt_uint_t idx=0; idx < 16; idx++) {
        xtnt_uint_t nkey = idx + 16;
        struct xtnt_node *inserted = xtnt_list_insert(&sets[0], &in_nodes[idx]);
        ck_assert_msg(inserted == &in_nodes[idx],
            "Expected node%u but got node%u", idx, inserted->key);
        ck_assert_msg(inserted->key == nkey,
            "Expected node%u with key %u, but got node.key=%u", idx, nkey, inserted->key);
        ck_assert_msg(sets[0].count == (idx + 17),
            "Expected set.count of %u, but got set.count=%u", (idx + 17), sets[0].count);
        ck_assert_msg(sets[0].link[XTNT_NODE_HEAD] == &in_nodes[idx],
            "Expected set head as node%u but got node%u", idx, sets[0].link[XTNT_NODE_HEAD]->key);
        ck_assert_msg(sets[0].link[XTNT_NODE_TAIL] == &nodes[15],
            "Expected set tail as node15 but got node%u", (sets[0].link[XTNT_NODE_TAIL]->key));
    }
}
END_TEST

START_TEST (test_xtnt_list_insert_nodes)
{
    for (xtnt_uint_t idx = 0; idx < 16; idx++) {
        xtnt_uint_t key = idx + 16;
        struct xtnt_node *inserted = xtnt_list_insert(&sets[1], &in_nodes[idx]);
        ck_assert_msg(inserted == &in_nodes[idx],
            "Expected node%u but got node%u", idx, inserted->key);
        ck_assert_msg(inserted->key == key,
            "Expected node%u with key %u, but got node.key=%u", idx, key, inserted->key);
        ck_assert_msg(sets[1].count == (idx + 1),
            "Expected set.count of %u, but got set.count=%u", (idx + 1), sets[1].count);
        ck_assert_msg(sets[1].link[XTNT_NODE_HEAD] == &in_nodes[idx],
            "Expected set head as node%u but got node%u", idx, (sets[1].link[XTNT_NODE_HEAD]->key - 16));
        ck_assert_msg(sets[1].link[XTNT_NODE_TAIL] == &in_nodes[0],
            "Expected set tail as node0 but got node%u", (sets[1].link[XTNT_NODE_TAIL]->key - 16));
    }
}
END_TEST

START_TEST (test_xtnt_list_replace_empty)
{
    for (xtnt_uint_t idx = 0; idx < 16; idx++) {
        struct xtnt_node* replaced = xtnt_list_replace(&sets[1], &in_nodes[idx], idx);
        ck_assert_msg(replaced == NULL,
            "Expected replaced to be NULL, but got node%u", replaced->key);
        ck_assert_msg(sets[1].count == 0,
            "Expected set.count as 0, but got %u", sets[1].count);
        ck_assert_msg(sets[1].link[XTNT_NODE_HEAD] == NULL,
            "Expected set head as NULL but got node%u", sets[1].link[XTNT_NODE_HEAD]->key);
        ck_assert_msg(sets[1].link[XTNT_NODE_TAIL] == NULL,
            "Expected set tail as NULL but got node%u", sets[1].link[XTNT_NODE_TAIL]->key);
    }
}
END_TEST

START_TEST (test_xtnt_list_replace_nodes)
{
    for (xtnt_uint_t idx = 0; idx < 16; idx++) {
        struct xtnt_node* replaced = xtnt_list_replace(&sets[0], &in_nodes[idx], idx);
        ck_assert_msg(replaced == &nodes[idx],
            "Expected found to be node%u, but got node%u", idx, replaced->key);
        ck_assert_msg(sets[0].count == 16,
            "Expected set.count as 16, but got %u", sets[0].count);
        if (idx < 15) {
            ck_assert_msg(sets[0].link[XTNT_NODE_HEAD] == &in_nodes[0],
                "Expected set head as node%u but got node%u", &in_nodes[idx].key, sets[0].link[XTNT_NODE_HEAD]->key);
            ck_assert_msg(sets[0].link[XTNT_NODE_TAIL] == &nodes[15],
                "Expected set tail as node15 but got node%u", sets[0].link[XTNT_NODE_TAIL]->key);
        } else {
            ck_assert_msg(sets[0].link[XTNT_NODE_HEAD] == &in_nodes[0],
                "Expected set head as node%u but got node%u", &in_nodes[idx].key, sets[0].link[XTNT_NODE_HEAD]->key);
            ck_assert_msg(sets[0].link[XTNT_NODE_TAIL] == &in_nodes[15],
                "Expected set tail as node%u but got node%u", &in_nodes[idx].key, sets[0].link[XTNT_NODE_TAIL]->key);
        }
    }
}
END_TEST

START_TEST (test_xtnt_list_get_empty)
{
    for (xtnt_uint_t idx = 0; idx < 16; idx++) {
        struct xtnt_node* found = xtnt_list_get(&sets[1], idx);
        ck_assert_msg(found == NULL,
            "Expected found to be NULL, but got node%u", found->key);
        ck_assert_msg(sets[1].count == 0,
            "Expected set.count as 0, but got %u", sets[1].count);
        ck_assert_msg(sets[1].link[XTNT_NODE_HEAD] == NULL,
            "Expected set head as NULL but got node%u", sets[1].link[XTNT_NODE_HEAD]->key);
        ck_assert_msg(sets[1].link[XTNT_NODE_TAIL] == NULL,
            "Expected set tail as NULL but got node%u", sets[1].link[XTNT_NODE_TAIL]->key);
    }
}
END_TEST

START_TEST (test_xtnt_list_get_nodes)
{
    for (xtnt_uint_t idx = 0; idx < 16; idx++) {
        struct xtnt_node* found = xtnt_list_get(&sets[0], idx);
        ck_assert_msg(found == &nodes[idx],
            "Expected found to be node%u, but got node%u", idx, found->key);
        ck_assert_msg(sets[0].count == 16,
            "Expected set.count as 16, but got %u", sets[0].count);
        ck_assert_msg(sets[0].link[XTNT_NODE_HEAD] == &nodes[0],
            "Expected set head as node0 but got node%u", sets[0].link[XTNT_NODE_HEAD]->key);
        ck_assert_msg(sets[0].link[XTNT_NODE_TAIL] == &nodes[15],
            "Expected set tail as node15 but got node%u", sets[0].link[XTNT_NODE_TAIL]->key);
    }
}
END_TEST

Suite * xtnt_list_suite(void)
{
    Suite *s;
    TCase *tc_xtnt_list;

    s = suite_create("xtnt_list");

    tc_xtnt_list = tcase_create("List");

    tcase_add_checked_fixture(tc_xtnt_list, setup, teardown);

    tcase_add_test(tc_xtnt_list, test_xtnt_list_delete_empty);
    tcase_add_test(tc_xtnt_list, test_xtnt_list_delete_up);
    tcase_add_test(tc_xtnt_list, test_xtnt_list_delete_down);
    tcase_add_test(tc_xtnt_list, test_xtnt_list_delete_node7);
    tcase_add_test(tc_xtnt_list, test_xtnt_list_delete_node8);

    tcase_add_test(tc_xtnt_list, test_xtnt_list_insert_full);
    tcase_add_test(tc_xtnt_list, test_xtnt_list_insert_nodes);

    tcase_add_test(tc_xtnt_list, test_xtnt_list_replace_empty);

    tcase_add_test(tc_xtnt_list, test_xtnt_list_get_empty);
    tcase_add_test(tc_xtnt_list, test_xtnt_list_get_nodes);

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
