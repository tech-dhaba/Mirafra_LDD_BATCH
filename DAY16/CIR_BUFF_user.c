#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE_PATH "/dev/CIR_BUFF_driver"
#define BUFFER_SIZE 256

void write_log(const char *log_message) {
    int fd = open(DEVICE_PATH, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open the device for writing");
        exit(EXIT_FAILURE);
    }

    if (write(fd, log_message, strlen(log_message)) < 0) {
        perror("Failed to write to the device");
    } else {
        printf("Log written: %s\n", log_message);
    }

    close(fd);
}

void read_log() {
    int fd = open(DEVICE_PATH, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open the device for reading");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);

    if (bytes_read < 0) {
        perror("Failed to read from the device");
    } else if (bytes_read == 0) {
        printf("No logs available.\n");
    } else {
        buffer[bytes_read] = '\0'; // Null-terminate the read data
        printf("Read log: %s\n", buffer);
    }

    close(fd);
}

int main() {
    int choice;
    char log_message[BUFFER_SIZE];

    while (1) {
        printf("\nLogger Application\n");
        printf("1. Write Log\n");
        printf("2. Read Log\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline character from input buffer

        switch (choice) {
        case 1:
            printf("Enter log message: ");
            fgets(log_message, BUFFER_SIZE, stdin);
            log_message[strcspn(log_message, "\n")] = '\0'; // Remove newline character
            write_log(log_message);
            break;

        case 2:
            read_log();
            break;

        case 3:
            printf("Exiting...\n");
            exit(EXIT_SUCCESS);

        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
