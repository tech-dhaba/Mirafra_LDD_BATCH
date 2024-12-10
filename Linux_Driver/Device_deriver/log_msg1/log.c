#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE_PATH "/dev/logger_device"

int main() {
    char buffer[256];
    int fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return 1;
    }

    // Write a message
    const char *msg = "Hello, Kernel!";
    write(fd, msg, strlen(msg));

    // Read the message
    int bytes_read = read(fd, buffer, sizeof(buffer));
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';
        printf("Log from kernel: %s\n", buffer);
    }

    close(fd);
    return 0;
}

