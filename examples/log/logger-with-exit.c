#include <extant/extant.h>
#include <pthread.h>
#include <stdio.h>

// Struct for data passing
struct my_formatter_data
{   
    int x;
    int y;
    int z;
};

// String format function for logging with data
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
    int status;
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
    for (int x=0; x<25; x++) {
        for (int y=0; y<25; y++) {
            for (int z=0; z<25; z++) {
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

    // ... Program encounters normal shutdown, and wants logger thread to exit
    res = xtnt_logger_exit(logger);
    result = pthread_join(logger_thread, (void **) &status);
    res = xtnt_logger_destroy(&logger);
    int val = (int) PTHREAD_CANCELED;
    printf("Result of thread was: %d, compare to PTHREAD_CANCELED: %d\n", status, val);
    return res;
}
