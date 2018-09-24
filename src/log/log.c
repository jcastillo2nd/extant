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

#include <extant/log.h>

/**
 * @brief Insert a log entry into the logger
 *
 * @param[in] logger The logger handling the entry
 * @param[in] entry The entry to be logged
 * @return XTNT_SUCCESS or errno error from underlying call
 *
 * @note The logging system is a producer/consumer model. Once a log entry
 * is created, the producer should not perform any operations ( e.g. reuse )
 * the entry after sending it to the logger with this method.
 */
xtnt_int_t xtnt_log(
    struct xtnt_logger *logger,
    struct xtnt_logger_entry *entry)
{
    xtnt_queue_push(&(logger->queue), &(entry->node));
    return XTNT_SUCCESS;
}

/**
 * @brief Allocate, ininitialize and return an xtnt_logger pointer
 *
 * @param[in] log The FILE pointer for an open log file
 * @param[in] filename A filename for log file
 * @return xtnt_logger pointer or NULL if error
 *
 * @note The logger is created regardless of the initialization attempt or
 * log files provided. Either the log FILE stream or writeable filename must
 * be provided. If the log file stream is invalid, this will result in errors
 * from the subsequent xtnt_log_process calls.
 */
struct xtnt_logger *xtnt_logger_create(
    FILE *log,
    const char *filename)
{
    struct xtnt_logger *res = malloc(sizeof(struct xtnt_logger));
    if(res != NULL){
        xtnt_int_t fail = xtnt_logger_initialize(res);
        if(fail == XTNT_SUCCESS){
            fail = pthread_mutex_lock(&(res->lock));
            if(fail == XTNT_SUCCESS){
                res->log = log;
                res->filename = (char *) filename;
                if (res->log == NULL){
                    if(res->filename != NULL){
                        res->log = fopen(filename, "a+");
                    }
                }
                fail = pthread_mutex_unlock(&(res->lock));
                if (fail){
                    XTNT_LOCK_SET_FAIL(res->state);
                }
            } else {
                XTNT_LOCK_SET_FAIL(res->state);
            }
        } else {
            XTNT_LOCK_SET_FAIL(res->state);
        }
    }
    return res;
}

/**
 * @brief Unitialize and deallocate an xtnt_logger
 *
 * @param[in] logger xtnt_logger to destroy
 */
void xtnt_logger_destroy(
    struct xtnt_logger *logger)
{
    xtnt_logger_uninitialize(logger);
    free(logger);
}

/**
 * @brief Allocate and initialize an xtnt_logger_entry
 *
 * @param[in] data_length Memory size allocated for data
 * @param[in] msg_length Memory size allocated for message
 * @param[in] level The log level for this entry
 * @return xtnt_logger_entry pointer or NULL on failure
 */
struct xtnt_logger_entry *xtnt_logger_entry_create(
    size_t data_length,
    size_t msg_length)
{
    xtnt_int_t fail = XTNT_SUCCESS;
    void *data = malloc(data_length);
    void *msg = malloc(msg_length);
    struct xtnt_logger_entry *entry = malloc(sizeof(struct xtnt_logger_entry));
    if (data == NULL || msg == NULL || entry == NULL){
        fail = XTNT_FAILURE;
    }
    if (fail == XTNT_SUCCESS){
        entry->fmt_fn = NULL;
        entry->data = data;
        entry->msg = msg;
        entry->msg_length = msg_length;
        entry->level = XTNT_LOG_LEVEL_DEFAULT;
        xtnt_node_initialize(&(entry->node), 0, entry);
    } else {
        free(data);
        free(msg);
        free(entry);
        entry = NULL;
    }
    return entry;
}

/**
 * @brief Uninitialize and deallocate an xtnt_logger_entry
 *
 * @param[in] entry xtnt_logger_entry to destroy
 *
 * @note The logging is a producer/consumer model. This should not be called
 * by any thread other than the logger consumer.
 */
void xtnt_logger_entry_destroy(
    struct xtnt_logger_entry *entry)
{
    xtnt_node_uninitialize(&(entry->node));
    free(entry->msg);
    free(entry->data);
    free(entry);
}

/**
 * @brief Initialize a xtnt_logger
 *
 * @param[in] logger The xtnt_logger to initialize
 * @return XTNT_SUCCESS or errno return values from called functions
 */
xtnt_int_t xtnt_logger_initialize(
    struct xtnt_logger *logger)
{
    xtnt_int_t fail = XTNT_SUCCESS;
    fail = pthread_mutex_init(&(logger->lock), NULL);
    if (fail){
        return fail;
    }
    fail = pthread_mutex_lock(&(logger->lock));
    if (fail){
        return fail;
    }
    logger->log = NULL;
    logger->filename = NULL;
    logger->default_level = XTNT_LOG_LEVEL_DEFAULT;
    xtnt_node_set_initialize(&(logger->queue));
    fail = pthread_mutex_unlock(&(logger->lock));
    return fail;
}

/**
 * @brief The logger consumer function
 *
 * @param[in] logger xtnt_logger to process
 *
 * @note This is the function used when initiating a thread for processing
 * a logger. This thread will yield when logger is empty, but otherwise must
 * be terminated (e.g. pthread_cancel & pthread_join )
 */
void xtnt_logger_process(
    struct xtnt_logger *logger)
{
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    while (1){
        struct xtnt_logger_entry *entry;
        struct xtnt_node *node = xtnt_queue_pop(&(logger->queue));
        if (node == NULL){
            pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
            pthread_testcancel();
            sched_yield();
            pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        } else {
            entry = (struct xtnt_logger_entry *) node->value;
            char * (*get_string)(void *) = entry->fmt_fn;
            fputs(get_string(entry), logger->log);
            xtnt_logger_entry_destroy(entry);
        }
    }
}

/**
 * @brief Uninitialize an xtnt_logger
 *
 * @param[in] logger xtnt_logger to uninitialize
 * @return XTNT_SUCCESS, 0 or error from called functions
 */
xtnt_int_t xtnt_logger_uninitialize(
    struct xtnt_logger *logger)
{
    xtnt_int_t fail = XTNT_SUCCESS;
    fail = pthread_mutex_lock(&(logger->lock));
    if (fail == XTNT_SUCCESS){
        logger->log = NULL;
        logger->filename = NULL;
        logger->default_level = XTNT_LOG_LEVEL_DEFAULT;
        xtnt_node_set_uninitialize(&(logger->queue));
        fail = pthread_mutex_unlock(&(logger->lock));
        if (fail == XTNT_SUCCESS){
            fail = pthread_mutex_destroy(&(logger->lock));
        } else {
            XTNT_LOCK_SET_FAIL(logger->state);
        }
    } else {
        XTNT_LOCK_SET_FAIL(logger->state);
    }
    return fail;
}

/**
 * @brief Change logger default log level
 *
 * @param[in] logger xtnt_logger to update
 * @param[in] level New default log level
 * @return XTNT_SUCCESS, 0 or error from called functions
 */
xtnt_int_t xtnt_logger_level_set(
    struct xtnt_logger *logger,
    xtnt_uint_t level)
{
    xtnt_int_t fail = pthread_mutex_lock(&(logger->lock));
    if (!fail){
        logger->default_level = level;
    }
    fail = pthread_mutex_unlock(&(logger->lock));
    return fail;
}
