#ifndef LOGGER_H
#define LOGGER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 5
#define LOG_MSG_SIZE 256

// Circular buffer for logs
static char logs[BUFFER_SIZE][LOG_MSG_SIZE];
static int head = 0;
static int tail = 0;
static int count = 0;

// Helper function to get current timestamp
void get_timestamp(char *timestamp, size_t len) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    snprintf(timestamp, len, "[%lld.%06ld] ", (long long)ts.tv_sec, ts.tv_nsec / 1000);
}

// Function to write log
int write_log(const char *log_message) {
    if (strlen(log_message) >= LOG_MSG_SIZE - 64) {
        fprintf(stderr, "Error: Log message too long.\n");
        return -1;
    }

    // Get the current timestamp
    char timestamp[64];
    get_timestamp(timestamp, sizeof(timestamp));

    // Create the log entry with the timestamp
    char log_entry[LOG_MSG_SIZE];
    snprintf(log_entry, sizeof(log_entry), "%s%s", __TIME__, log_message);

    // Write to the circular buffer
    snprintf(logs[head], LOG_MSG_SIZE, "%s", log_entry);
    head = (head + 1) % BUFFER_SIZE;
    if (count < BUFFER_SIZE) {
        count++;
    } else {
        tail = (tail + 1) % BUFFER_SIZE;  // Overwrite the oldest log
    }

    //printf("Log written: %s\n", log_entry);
    return 0;
}

// Function to read log
int read_log(char *log_message, size_t len) {
    if (count == 0) {
        fprintf(stderr, "Error: No logs available.\n");
        return -1;
    }

    int log_len = strlen(logs[tail]);
    if (log_len > len) {
        fprintf(stderr, "Error: Buffer size too small to read log.\n");
        return -1;
    }

    snprintf(log_message, len, "%s", logs[tail]);
    tail = (tail + 1) % BUFFER_SIZE;
    count--;

    //printf("Log read: %s\n", log_message);
    return 0;
}

/*int main() {
    char user_input[LOG_MSG_SIZE];
    char log_message[LOG_MSG_SIZE];

    // Example of writing log messages
    write_log("a logger function calling 1st time \n");
    // Try reading logs from the circular buffer
    for (int i = 0; i < count; i++) {
	    if((i == BUFFER_SIZE) || (i==count) || (i==tail))
	    {
		    write_log("no logs availble \n");
	    }
        if (read_log(log_message, sizeof(log_message)) == 0) {
            printf("Log message: %s\n", log_message);
        }
    }

    return 0;
}*/
#endif
