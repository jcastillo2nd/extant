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
 * @retval XTNT_ESUCCESS on successful queue of entry to logger
 * @retval return value of `pthread_mutex_lock()` or `pthread_mutex_unlock()`
 *
 * @note The logging system is a producer/consumer model. Once a log entry is
 * queued with a logger, the producer should consider the entry immutable.
 */
xtnt_status_t
xtnt_log(
    struct xtnt_logger *logger,
    struct xtnt_logger_entry *entry)
{
    return xtnt_queue_push(&(logger->queue), &(entry->node));
}

/**
 * @brief Change an existing logger level
 *
 * @param[in] logger Logger reference to update
 * @param[in] default_level New default level to set
 * @retval Status of pthread_mutex operations or XTNT_ESUCCESS
 */
xtnt_status_t
xtnt_logger_change_default_level(
    struct xtnt_logger *logger,
    xtnt_uint_t default_level)
{
    xtnt_status_t res = XTNT_EFAILURE;
    if ((res = pthread_mutex_lock(&(logger->lock))) == XTNT_ESUCCESS) {
        logger->default_level = default_level;
        if ((res = pthread_mutex_unlock(&(logger->lock))) != XTNT_ESUCCESS) {
            XTNT_LOCK_SET_UNLOCK_FAIL(logger->state);
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(logger->state);
    }
    return res;
}

/**
 * @brief Allocate, ininitialize and return an xtnt_logger pointer
 *
 * @param[in] log The FILE pointer for an open log file
 * @param[in] filename A filename for log file
 * @param[out] logger Pointer reference to store logger to
 * @retval XTNT_ESUCCESS on allocation and initialization
 * @retval status return of xtnt_logger_initialize
 * @retval EINVAL on NULL log and NULL filename
 * @retval errno on `fopen()`, `malloc()`, `pthread_mutex_lock()` or
 * `pthread_mutex_unlock()`
 *
 * @note The logger is created regardless of the initialization attempt or
 * log files provided. Either the log FILE stream or writeable filename must
 * be provided. If the log file stream is invalid, this will result in errors
 * from the subsequent xtnt_log_process calls.
 */
xtnt_status_t
xtnt_logger_create(
    FILE *log,
    const char *filename,
    struct xtnt_logger **logger)
{
    xtnt_status_t res = XTNT_ESUCCESS;
    struct xtnt_logger *mlogger = malloc(sizeof(struct xtnt_logger));
    if (mlogger != NULL) {
        if ((res = xtnt_logger_initialize(mlogger)) == XTNT_ESUCCESS) {
            if ((res = pthread_mutex_lock(&(mlogger->lock))) == XTNT_ZERO) {
                mlogger->log = log;
                mlogger->filename = (char *) filename;
                if (mlogger->log == NULL){
                    if(mlogger->filename != NULL){
                        mlogger->log = fopen(filename, "a+");
                        if (mlogger->log == NULL) {
                            res = errno;
                        }
                    } else {
                        res = EINVAL;
                    }
                }
                if ((res = pthread_mutex_unlock(&(mlogger->lock)) != XTNT_ZERO)){
                    XTNT_LOCK_SET_UNLOCK_FAIL(mlogger->state);
                }
            } else {
                XTNT_LOCK_SET_LOCK_FAIL(mlogger->state);
            }
        } else {
            res = xtnt_logger_uninitialize(mlogger);
            free(mlogger);
            mlogger = NULL;
        }
    } else {
        res = errno;
    }
    *logger = mlogger;
    return res;
}

/**
 * @brief Uninitialize and deallocate an xtnt_logger
 *
 * @param[in] logger xtnt_logger to destroy
 * @returns result of xtnt_logger_uninitialize
 */
xtnt_status_t
xtnt_logger_destroy(
    struct xtnt_logger **logger)
{
    xtnt_status_t res = XTNT_EFAILURE;
    res = xtnt_logger_uninitialize(*logger);
    free(logger);
    *logger = NULL;
    return res;
}

/**
 * @brief Allocate and initialize an xtnt_logger_entry
 *
 * @param[in] data_length Memory size allocated for data
 * @param[in] msg_length Memory size allocated for message
 * @param[in] level Level for log entry
 * @param[in] fmt_fn Function for formatting string
 * @param[out] entry Pointer reference to store entry to
 * @retval XTNT_EFAILURE on allocation failure
 * @retval XTNT_ESUCCESS on allocation and initialization
 * @retval status of xtnt_node_initialize
 * @retval errno of malloc
 */
xtnt_status_t
xtnt_logger_entry_create(
    size_t data_length,
    size_t msg_length,
    void *fmt_fn,
    xtnt_uint_t level,
    struct xtnt_logger_entry **entry)
{
    xtnt_status_t res = XTNT_EFAILURE;
    void *data = malloc(data_length);
    void *msg = malloc(msg_length);
    *entry = malloc(sizeof(struct xtnt_logger_entry));
    if (data != NULL || msg != NULL || entry != NULL){
        (*entry)->fmt_fn = fmt_fn;
        (*entry)->data = data;
        (*entry)->msg = msg;
        (*entry)->msg_length = msg_length;
        (*entry)->level = level;
        (*entry)->level = XTNT_LOG_LEVEL_DEFAULT;
        if ((res = xtnt_node_initialize(&((*entry)->node), level, 0, *entry)) != XTNT_ESUCCESS) {
            XTNT_STATE_SET_VALUE((*entry)->state, XTNT_LOG_ENTRY_INIT_FAIL);
        }
    } else {
        free(entry);
        free(msg);
        free(data);
        *entry = NULL;
        res = errno;
    }
    return res;
}

/**
 * @brief Uninitialize and deallocate an xtnt_logger_entry
 *
 * @param[in] entry xtnt_logger_entry to destroy
 * @returns result of xtnt_node_uninitialize
 *
 * @note The logging is a producer/consumer model. This should not be called
 * by any thread other than the logger consumer.
 */
xtnt_status_t
xtnt_logger_entry_destroy(
    struct xtnt_logger_entry **entry)
{
    xtnt_status_t res = XTNT_ESUCCESS;
    res = xtnt_node_uninitialize(&((*entry)->node));
    free((*entry)->msg);
    free((*entry)->data);
    free(*entry);
    *entry = NULL;
    return res;
}

/**
 * @brief Initialize a xtnt_logger
 *
 * @param[in] logger The xtnt_logger to initialize
 * @returns result of mutex operations
 */
xtnt_status_t
xtnt_logger_initialize(
    struct xtnt_logger *logger)
{
    xtnt_status_t res = XTNT_EFAILURE;
    if ((res = pthread_mutex_init(&(logger->lock), NULL)) == XTNT_ZERO) {
        if ((res = pthread_mutex_lock(&(logger->lock))) == XTNT_ZERO) {
            logger->log = NULL;
            logger->filename = NULL;
            logger->default_level = XTNT_LOG_LEVEL_DEFAULT;
            /** @todo upate xtnt_node_set_initialize to return xtnt_status_t */
            if ((res = xtnt_node_set_initialize(&(logger->queue))) != XTNT_ZERO) {
                XTNT_STATE_SET_VALUE(logger->state, XTNT_LOG_ENTRY_INIT_FAIL);
            }
            if ((res = pthread_mutex_unlock(&(logger->lock))) != XTNT_ZERO) {
                XTNT_LOCK_SET_UNLOCK_FAIL(logger->state);
            }
        } else {
            XTNT_LOCK_SET_LOCK_FAIL(logger->state);
        }
    } else {
        XTNT_LOCK_SET_INIT_FAIL(logger->state);
    }
    return res;
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
void
xtnt_logger_process(
    struct xtnt_logger *logger)
{
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

    xtnt_uint_t iter = XTNT_LOG_BATCH_SIZE;
    xtnt_status_t res = XTNT_ESUCCESS;

    while (1){
        struct xtnt_logger_entry *entry= NULL;
        struct xtnt_node *node = NULL;
        xtnt_int_t error = errno;

        pthread_mutex_lock(&(logger->lock));
        xtnt_uint_t level = logger->default_level;
        xtnt_uint_t state = XTNT_STATE(logger->state);
        pthread_mutex_unlock(&(logger->lock));

        if (state == XTNT_LOGGER_PENDING_EXIT) {
            XTNT_STATE_SET_VALUE(logger->state, XTNT_LOGGER_COMPLETED_EXIT);
        } else {
            res = xtnt_queue_pop(&(logger->queue), &node);
        }

        if (node == NULL || !(iter)) {
            iter = XTNT_LOG_BATCH_SIZE;
            fflush(logger->log);
            pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
            pthread_testcancel();
            pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
            sched_yield();
        } else {
            entry = (struct xtnt_logger_entry *) node->value;
            char * (*get_string)(struct xtnt_logger_entry *) = entry->fmt_fn;

            /**
             * @todo Review need for optimization for lock here.
             */
            if (entry->level & level) {
                if(XTNT_IS_EFAILURE((res = fputs(get_string(entry), logger->log)))) {
                    XTNT_STATE_SET_VALUE(logger->state, XTNT_LOGGER_WRITE_FAIL);
                    pthread_exit(&error);
                }
            }
            xtnt_logger_entry_destroy(&entry);
            iter--;
        }
    }
}

/**
 * @brief Uninitialize an xtnt_logger
 *
 * @param[in] logger xtnt_logger to uninitialize
 * @return XTNT_ESUCCESS or error from subfunctions
 */
xtnt_status_t
xtnt_logger_uninitialize(
    struct xtnt_logger *logger)
{
    xtnt_status_t res = XTNT_EFAILURE;
    if ((res = pthread_mutex_lock(&(logger->lock))) == XTNT_ZERO) {
        xtnt_node_set_uninitialize(&(logger->queue));
        if ((res = pthread_mutex_unlock(&(logger->lock))) == XTNT_ZERO) {
            if ((res = pthread_mutex_destroy(&(logger->lock))) != XTNT_ZERO) {
                XTNT_LOCK_SET_DESTROY_FAIL(logger->state);
            }
        } else {
            XTNT_LOCK_SET_UNLOCK_FAIL(logger->state);
        }
    } else {
        XTNT_LOCK_SET_LOCK_FAIL(logger->state);
    }
    return res;
}
