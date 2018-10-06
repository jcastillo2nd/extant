# The logger system # {#loggersystem}

The libextant logging system was developed with considerations for offloading
string conversion and writing to a separate thread using a producer/consumer
model. In general, memory allocation and data assignment speed is assumed to be
faster than string formatting and blocking write calls.

# The xtnt_logger # {#logger}

The [logger](@ref xtnt_logger) itself is allocated and set with a 
[default level](@ref xtnt_logger::default_level). This can be changed by any
thread using [xtnt_logger_level_set](@ref xtnt_logger_level_set) otherwise it
uses the [default log level](@ref XTNT_LOG_LEVEL_DEFAULT). It's possible for any application to have multiple
loggers, however it is up to the application to ensure that entries are
directed to the correct loggers. Any application with a single logger can limit
the logging by simply adjusting the level when creating entries for the logger.

Here is an example of initializing a logger and creating a thread to process
the requests:

```{.c}
xtnt_status_t res = XTNT_ESUCCESS;
pthread_t logger_thread;
struct xtnt_logger *logger = NULL;

res = xtnt_logger_create(NULL, XTNT_LOG_DEFAULT_PATH, &logger);
if (res == XTNT_ESUCCESS) {
    res = pthread_create(&logger_thread, NULL, (void *) xtnt_logger_process_thread_fn, logger);
} else {
    // Handle any failures or special conditions
}
// Check for errors and continue with main application
```

Once the logger thread has started, **no other actions on the logger should be
performed**. 

It's worth noting that the logger processes the queued entries, but yields to
other threads using the `sched_yield()` call. During the time it is processing,
the thread cancel state is set to `PTHREAD_CANCEL_DISABLE`. The logger yields
on and can be cancelled on essentially two conditions:

* The logger has 0 entries to process
* The logger has processed `XTNT_LOG_BATCH_SIZE` entries

This implementation is an attempt to ensure that the thread has an opportunity
to log all entries prior to being ended.

The producer threads or main application are responsible for generating log
entries by calling the `xtnt_log()` function with both the logger pointer and
entry pointer:

```{.c}
res = xtnt_log(logger, entry);
```

A status is returned, and essentially is the result of pushing the entry to the
logging queue. If this fails for any reason, special handling of the logger
queue would be possible, depending on the state of the `xtnt_node_set` the
queue is stored in.

**If any operation on the queue must be done outside of the logger processing
thread, `pthread_mutex_lock()` should be called on the queue to ensure thread
safety.**

# The xtnt_logger_entry # {#entry}

Formatting strings and writing them to a log are relatively longer running
operations when compared to memory allocation and assignment operations. This
is the main idea driving the logger system.

As mentioned in the [previous](#logger) section, the consumer object is
typically run as a separate thread and operates a queue with the log entries
added to it. The entries are pushed to the queue via the `xtnt_log()` function
and the logger thread continually formats and writes out the entries.

The ability to defer the formatting however requires some work. This involves
the definition of [data](@ref xtnt_logger_entry::data), the definition of a
[function](@ref xtnt_logger_entry::fmt_fn) that can work with that data and a
[level](@ref xtnt_logger_entry::level) designation for an entry.

These details are wrapped in a self-referential node that is added to the queue
of the logger. The node's value points to the entry itself, which includes the
node.

Because log messages vary in length, the entry must be defined with a limit to
the length of the message. The software defines a default of 128, but can be
extended for any entry when the entry is created.

Creating the entry looks rather simple:

```{.c}
xtnt_status_t res = XTNT_ESUCCESS;
xtnt_logger_entry *entry = NULL;
res = xtnt_logger_entry_create(
        sizeof(struct msgdata),
        XTNT_LOG_MSG_DEFAULT_SIZE,
        msgformatter,
        XTNT_LOG_LEVEL_INFO,
        &entry);
```

There is considerable work to establish the
[data](@ref xtnt_logger_entry::data) and 
[fmt_fn](@ref xtnt_logger_entry::fmt_fn) however.

## The data ##

The data ( typically a structure ) is allocated at the time of logger entry
creation. This is then populated by the caller before submitting the entry via
`xtnt_log()` to the logger. The actual structure to or contents of the data is
insignificant to the logger.

In practice, the data should be considered static once provided to the logger.
This is to say that any data should include the relevant values stored into it
and not rely on dereferencing from the data:

```{.c}
struct msgdata
{
    float a;
    float b;
    float c;
    float d;
};

// ... with logger created as *logger, and 2 entries created as *entry1 and *entry2


// Example using a cast to dereference
(struct msgdata *) (entry1->data)->a = 4.0;
(struct msgdata *) (entry1->data)->b = 3.0;
(struct msgdata *) (entry1->data)->c = 2.0;
(struct msgdata *) (entry1->data)->d = 1.0;

// Example using a variable to dereference
struct msgdata *data2 = NULL;
data2 = entry2->data;
data2->a = 1.0;
data2->b = 2.0;
data2->c = 3.0;
data2->d = 4.0;

xtnt_log(logger, entry1);
xtnt_log(logger, entry2);
```

The contents are referenced by the [fmt_fn](@ref xtnt_logger_entry::fmt_fn)
function which is also defined by the calling application.

## The format function ##

The formatting is also offloaded to the logger thread. This is done by way of
function pointer. This is set at creation with `xtnt_logger_entry_create()`.
The ideal formatter would not require anything more than referencing the data
within a call to the string formatting method.

The [entry](@ref xtnt_logger_entry) itself is passed to the formatter, which
provides access to the all the data available in the entry. Most important is
the **agreement of data passed in with the operations performed** within the
formatting function:

```{.c}
#define MSGSTRING "We are logging our float msg, a=%f, b=%f, c=%f, d=%f\n"

char *
msgformatter(struct xtnt_logger_entry *e)
{
    struct msgdata *d = e->data;
    snprintf(e->msg, e->msg_length, MSGSTRING, d->a, d->b, d->c, d->d);
    return e->msg;
}
```

There isn't any specific convention for logging. The logging levels have no
impact on the output. **The [formatter function](@ref xtnt_logger_entry::fmt_fn) is entirely responsible for generating
the style of logging** written out and this is implemented by the calling
application.

Using `case`, preprocessor macros such as `__LINE__` or `__FILE__`, or calling
subfunctions in the formatter to generate logs with prefixes, particular
styling or markup output are reasonable implementations. And these are all
fully within the control of the calling application.

# Some considerations #

As the logging system was designed to run in a separate thread, general rules
for thread safety are:

* A [logger](@ref xtnt_logger) is opaque

* Never [destroy](@ref xtnt_logger_destroy) a [logger](@ref xtnt_logger) before
  `pthread_cancel()` **and** `pthread_join()`

* An [entry](@ref xtnt_logger_entry) passed to [xtnt_log()](@ref xtnt_log) is
  immutable

* An entry is never [destroyed](@ref xtnt_logger_entry_destroy)

* The [logger entry data](@ref xtnt_logger_entry::data) should not include
  pointers

* The [logger entry formatter](@ref xtnt_logger_entry::fmt_fn) should not
  dereference anything from [data](@ref xtnt_logger_entry::data)

* The [logger queue](@ref xtnt_logger::queue) should not be modified outside of
  the consumer thread without first locking it, **which will also block the
  consumer thread**.

Finally, this is only a logging system. For more advanced handling, such as
asynchronous I/O, consider using another library such as libevent or libev in
the main application.

# Examples #

Here we have a few example cases that can be used as references for the library
usage.

## Creating a threaded logger ##

In this example, we have a program that initiates a logger, generates entries
and passes them to the the logger before cancelling the logger and exiting.

```{.c}
#include <extant/extant.h>
#include <pthread.h>
#include <stdio.h>

// Convenient struct for data passing
struct my_formatter_data
{   
    int x;
    int y;
    int z;
};

// Prepares the string for logging
char *
my_formatter (struct xtnt_logger_entry *entry)
{   
    struct my_formatter_data *d = entry->data;
    snprintf(
        entry->msg,
        entry->msg_length,
        "My message: X: %d, Y: %d, Z: %d\n",
        d->x,
        d->y,
        d->z);
    return entry->msg;
}

int main(int argc, char **argv)
{
    // Thread setup
    pthread_t logger_thread;
    xtnt_int_t *status;
    xtnt_int_t result;

    // Logger and error setup
    xtnt_status_t res;
    struct xtnt_logger *logger;
    if ((res = xtnt_logger_create(NULL, XTNT_LOG_PATH, &logger)) != XTNT_ESUCCESS) {
        fprintf(stderr, "Failed to create logger");
        return -1;
    }

    // Start a logger consumer in a thread
    result = pthread_create(&logger_thread, NULL, (void *) xtnt_logger_process, logger);

    // Start logging
    for (int x=0; x<10; x++) {
        for (int y=0; y<100; y++) {
            for (int z=0; z<100; z++) {
                struct xtnt_logger_entry *entry;
                res = XTNT_EFAILURE;
                if ((res = xtnt_logger_entry_create(sizeof(struct my_formatter_data), sizeof(char) * 128, &my_formatter, XTNT_LOG_INFO, &entry)) == XTNT_ESUCCESS) {
                    ((struct my_formatter_data *) entry->data)->x = x;
                    ((struct my_formatter_data *) entry->data)->y = y;
                    ((struct my_formatter_data *) entry->data)->z = z;

                    if ((res = xtnt_log(logger, entry)) != XTNT_ESUCCESS) {
                        fprintf(stderr, "Failed to write to logger, entry {%d-%d-%d}\n", x, y, z);
                    }
                } else {
                    fprintf(stderr, "Failed to initialize logger entry {%d-%d-%d}\n", x, y, z);
                }
            }
        }
    }

    // Wait for all the logs to be written out
    result = pthread_cancel(logger_thread);
    result = pthread_join(logger_thread, (void **) &status);
    printf("Result of thread was: %d, compare to PTHREAD_CANCELED: %d\n", status, PTHREAD_CANCELED);
    return XTNT_ZERO;
}
```

## Logging through Macros ##

It's worth noting that using defines, macro expansion and collections of common
data and formatters in a single header file could be used to manage logging.

```{.c}
struct my_formatter_data
{   
    int x;
    int y;
    int z;
};

char *
my_formatter (struct xtnt_logger_entry *entry)
{   
    struct my_formatter_data *d = entry->data;
    snprintf(
        entry->msg,
        entry->msg_length,
        "My message: X: %d, Y: %d, Z: %d\n",
        d->x,
        d->y,
        d->z);
    return entry->msg;
}

#define LOG_3DPOINT(logger, x, y, z) \
do { \
    struct xtnt_logger_entry *entry; \
    res = XTNT_EFAILURE; \
    if ((res = xtnt_logger_entry_create(sizeof(struct my_formatter_data), sizeof(char) * 128, &my_formatter, XTNT_LOG_INFO, &entry)) == XTNT_ESUCCESS) { \
        ((struct my_formatter_data *) entry->data)->x = x; \
        ((struct my_formatter_data *) entry->data)->y = y; \
        ((struct my_formatter_data *) entry->data)->z = z; \
        if ((res = xtnt_log(logger, entry)) != XTNT_ESUCCESS) { \
            fprintf(stderr, "Failed to write to logger, entry {%d-%d-%d}\n", x, y, z); \
        } \
    } \
} while (0)
```

The earlier example would then reduce the block for logging to:

```{.c}
// ...
    for (int x=0; x<10; x++) {
        for (int y=0; y<100; y++) {
            for (int z=0; z<100; z++) {
                LOG_3DPOINT(logger, x, y, z);
            }
        }
    }
// ...
```

## Getting a logger to exit ##

If for any reason you need a logger to exit, you can cause it to exit and check
the state.

```{.c}
// ... Logger and entries created, but need to exit logger for some reason
pthread_mutex_lock(&(logger->lock)); // Blocks until logger is free
XTNT_STATE_SET_VALUE(logger->state, XTNT_LOGGER_PENDING_EXIT); // Set's the state for the logger to "Pending exit"
pthread_mutex_unlock(&(logger->lock)); // Returns control to the thread, which will read status and exit
result = pthread_join(logger_thread, (void **) &status); // Block until the thread exits
// ... The logger thread will have set the state to XTNT_LOGGER_COMPLETED_EXIT and returned
```
