#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define LOG_FILE "error_log.txt"

// Macro for logging errors
#define LOG_ERROR(message) log_error(__FILE__, __LINE__, __func__, message)

void log_error(const char *file, int line, const char *function, const char *message);

#endif

void some_function() {
    LOG_ERROR("An error occurred in some_function");
}

int main() {
    LOG_ERROR("An error occurred in main");
    some_function();
    return 0;
}

void log_error(const char *file, int line, const char *function, const char *message) {
    // Open the log file in append mode
    int log_fd = open(LOG_FILE, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (log_fd == -1) {
        perror("Failed to open log file");
        return;
    }

    // Prepare the log message
    char buffer[512];
    int len = snprintf(buffer, sizeof(buffer),
                       "[%s %s] ERROR: %s\n\tFile: %s\n\tLine: %d\n\tFunction: %s\n\n",
                       __DATE__, __TIME__, message, file, line, function);

    // Write the log message to the file
    if (write(log_fd, buffer, len) == -1) {
        perror("Failed to write to log file");
    }

    // Close the log file
    close(log_fd);
}
