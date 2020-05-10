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
#include <extant/set/queue.h>

#include <stdio.h>

struct xtnt_node node1q1, node2q1, node3q1, node1q2, node2q2, node3q2;
struct xtnt_node_set queue1, queue2;
xtnt_uint_t value1, value2, value3;

void setup(void)
{
    node1q1.key = 1;
    node1q1.value = &value1;
    node1q1.link[XTNT_NODE_HEAD] = &node2q1;
    node1q1.link[XTNT_NODE_MIDDLE] = \
    node1q1.link[XTNT_NODE_TAIL] = NULL;

    node2q1.key = 2;
    node2q1.value = &value2;
    node2q1.link[XTNT_NODE_HEAD] = \
    node2q1.link[XTNT_NODE_MIDDLE] = NULL;
    node2q1.link[XTNT_NODE_TAIL] = &node1q1;

    node3q1.key = 3;
    node3q1.value = &value3;
    node3q1.link[XTNT_NODE_HEAD] = \
    node3q1.link[XTNT_NODE_MIDDLE] = \
    node3q1.link[XTNT_NODE_TAIL] = NULL;

    queue1.root.link[XTNT_NODE_HEAD] = &node2q1;
    queue1.root.link[XTNT_NODE_MIDDLE] = NULL;
    queue1.root.link[XTNT_NODE_TAIL] = &node1q1;
    queue1.count = 2;

    node1q2.key = 1;
    node1q2.value = &value1;
    node1q2.link[0] = node1q2.link[1] = node1q2.link[2] = NULL;

    node2q2.key = 2;
    node2q2.value = &value2;
    node2q2.link[0] = node2q2.link[1] = node2q2.link[2] = NULL;

    node3q2.key = 3;
    node3q2.value = &value3;
    node3q2.link[0] = node3q2.link[1] = node3q2.link[2] = NULL;

    queue2.root.link[XTNT_NODE_HEAD] = NULL;
    queue2.root.link[XTNT_NODE_MIDDLE] = NULL;
    queue2.root.link[XTNT_NODE_TAIL] = NULL;
    queue2.count = 0;
}

void teardown(void)
{
}

START_TEST (test_xtnt_queue_peek)
{
    struct xtnt_node *node = NULL;
    xtnt_status_t res = xtnt_queue_peek(&queue1, &node);
    ck_assert_msg(res == XTNT_ESUCCESS,
        "Expected xtnt_queue_peek to succeed");
    ck_assert_msg(node->key == 1,
        "Expected node of key 1 but got %u", node->key); 
}
END_TEST

START_TEST (test_xtnt_empty_queue_peek)
{
    struct xtnt_node *node = NULL;
    xtnt_status_t res = xtnt_queue_peek(&queue2, &node);
    ck_assert_msg(res == XTNT_ESUCCESS,
        "Expected xtnt_queue_peek to succeed");
    ck_assert_msg(node == NULL,
        "Expected empty node, but received node.key=%u", node->key);
}
END_TEST

START_TEST (test_xtnt_queue_pop)
{
    struct xtnt_node *node = NULL;
    xtnt_status_t res = xtnt_queue_pop(&queue1, &node);
    ck_assert_msg(res == XTNT_ESUCCESS,
        "Expected xtnt_queue_pop to succeed");
    ck_assert_msg(node->key == 1,
        "Expected node with key 1, but received node.key=%u", node->key);
}
END_TEST

START_TEST (test_xtnt_empty_queue_pop)
{
    struct xtnt_node *node = NULL;
    xtnt_status_t res = xtnt_queue_pop(&queue2, &node);
    ck_assert_msg(res == XTNT_ESUCCESS,
        "Expected xtnt_queue_pop to succeed");
    ck_assert_msg(node == NULL,
        "Expected empty node, but received node.key=%u", node->key);
}
END_TEST

START_TEST (test_xtnt_queue_push)
{
    xtnt_status_t res = xtnt_queue_push(&queue1, &node3q1);
    struct xtnt_node *head = queue1.root.link[XTNT_NODE_HEAD];
    ck_assert_msg(res == XTNT_ESUCCESS,
        "Expected xtnt_queue_push to succeed");
    ck_assert_msg(head->key == (&node3q1)->key,
        "Expected queue with node3 as tail, but have node.key=%u", head->key);
}
END_TEST

START_TEST (test_xtnt_empty_queue_push)
{
    xtnt_status_t res = xtnt_queue_push(&queue2, &node1q2);
    struct xtnt_node *tail = queue1.root.link[XTNT_NODE_TAIL];
    ck_assert_msg(res == XTNT_ESUCCESS,
        "Expected xtnt_queue_push to succeed");
    ck_assert_msg(tail->key == (&node1q2)->key,
        "Expected queue with tail.key as node.key, but have tail.key=%u", tail->key);
}
END_TEST

Suite * xtnt_queue_suite(void)
{
    Suite *s;
    TCase *tc_xtnt_queue;

    s = suite_create("xtnt_queue");

    tc_xtnt_queue = tcase_create("Queue");

    tcase_add_checked_fixture(tc_xtnt_queue, setup, teardown);
    tcase_add_test(tc_xtnt_queue, test_xtnt_queue_peek);
    tcase_add_test(tc_xtnt_queue, test_xtnt_empty_queue_peek);
    tcase_add_test(tc_xtnt_queue, test_xtnt_queue_pop);
    tcase_add_test(tc_xtnt_queue, test_xtnt_empty_queue_pop);
    tcase_add_test(tc_xtnt_queue, test_xtnt_queue_push);
    tcase_add_test(tc_xtnt_queue, test_xtnt_empty_queue_push);
    suite_add_tcase(s, tc_xtnt_queue);

    return s;
}

int main(void)
{
    int failed;
    Suite *s;
    SRunner *sr;

    s = xtnt_queue_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? 0 : failed;
}
