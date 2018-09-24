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

#include <extant/set/queue.h>

struct xtnt_node *xtnt_queue_peek(
        struct xtnt_node_set *queue)
{
    struct xtnt_node *peek = NULL;
    xtnt_uint_t fail = pthread_mutex_lock(&(queue->lock));
    if (fail) {
        XTNT_LOCK_SET_FAIL(queue->state);
		return NULL;
	}
	if (queue->link[XTNT_NODE_TAIL] != NULL){
        peek = queue->link[XTNT_NODE_TAIL];
	}
	fail = pthread_mutex_unlock(&(queue->lock));
        if (fail){
            XTNT_LOCK_SET_FAIL(queue->state);
	}
    return peek;
}

struct xtnt_node *xtnt_queue_pop(
        struct xtnt_node_set *queue)
{
    struct xtnt_node *popped = NULL;
    xtnt_uint_t fail = pthread_mutex_lock(&(queue->lock));
    if (fail) {
        XTNT_LOCK_SET_FAIL(queue->state);
        return NULL;
    }
    if (queue->link[XTNT_NODE_TAIL] != NULL){
        popped = queue->link[XTNT_NODE_TAIL];
        // Use of size_t lead to any bugs?
        if ((size_t) queue->link[XTNT_NODE_HEAD] ^
            (size_t) queue->link[XTNT_NODE_TAIL]){
            popped->link[XTNT_NODE_HEAD]->link[XTNT_NODE_TAIL] = NULL;
            queue->link[XTNT_NODE_TAIL] = popped->link[XTNT_NODE_HEAD];
        } else {
            queue->link[XTNT_NODE_TAIL] = NULL;
            queue->link[XTNT_NODE_HEAD] = NULL;
        }
        queue->count--;
    }
    fail = pthread_mutex_unlock(&(queue->lock));
    if (fail){
        XTNT_LOCK_SET_FAIL(queue->state);
    }
    return popped;
}

struct xtnt_node *xtnt_queue_push(
        struct xtnt_node_set *queue,
        struct xtnt_node *node)
{
    xtnt_int_t fail = pthread_mutex_lock(&(queue->lock));
    if (fail) {
        XTNT_LOCK_SET_FAIL(queue->state);
        return NULL;
    }
    if (queue->link[XTNT_NODE_HEAD] != NULL) {
        queue->link[XTNT_NODE_HEAD]->link[XTNT_NODE_HEAD] = node;
        node->link[XTNT_NODE_TAIL] = queue->link[XTNT_NODE_HEAD];
        queue->link[XTNT_NODE_HEAD] = node;
    } else {
        queue->link[XTNT_NODE_TAIL] = node;
        queue->link[XTNT_NODE_HEAD] = node;
    }
    queue->count++;
    fail = pthread_mutex_unlock(&(queue->lock));
    if (fail) {
        XTNT_LOCK_SET_FAIL(queue->state);
    }
    return node;
}
