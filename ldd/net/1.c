#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_PATH "/sys/class/net/mynet0"

int main() {
    int fd;
    char message[] = "Hello from User Space";

    // Open the device
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return -1;
    }

    // Write to the device (simulating a request)
    if (write(fd, message, strlen(message)) < 0) {
        perror("Failed to write to the device");
        close(fd);
        return -1;
    }

    // Close the device
    close(fd);
    return 0;
}

