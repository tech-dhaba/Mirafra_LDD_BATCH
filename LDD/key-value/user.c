#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define DEVICE_PATH "/dev/kv_store"
#define MAX_KEY_LEN 32
#define MAX_VALUE_LEN 32

int main() {
    int fd;
    char buffer[64];
    ssize_t bytes_written, bytes_read;
    char key[MAX_KEY_LEN];

    // Open the device file
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd == -1) {
        perror("Failed to open device file");
        return -1;
    }

    // Write 5 key-value pairs to the device
    printf("Enter 5 key-value pairs (key,value):\n");
    for (int i = 0; i < 5; i++) {
        printf("Enter key-value pair %d: ", i + 1);
        fgets(buffer, sizeof(buffer), stdin);

        bytes_written = write(fd, buffer, strlen(buffer));
        if (bytes_written == -1) {
            perror("Failed to write to device");
            close(fd);
            return -1;
        }
        printf("Key-value pair written: %s\n", buffer);
    }

    // Read the value of a key
    printf("Enter a key to fetch its value: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = 0;  // Remove the newline character

    bytes_written = write(fd, key, strlen(key));
    if (bytes_written == -1) {
        perror("Failed to write key to device");
        close(fd);
        return -1;
    }

    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("Failed to read from device");
        close(fd);
        return -1;
    }

    buffer[bytes_read] = '\0';  // Null-terminate the buffer
    printf("Returned: %s\n", buffer);

    close(fd);
    return 0;
}

