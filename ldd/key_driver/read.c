#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_FILE "/dev/key_value_driver"

int main() {
    int fd;
    char output_buffer[256];
    int key;

    /* Open the device file  */
    fd = open(DEVICE_FILE, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open device");
        return EXIT_FAILURE;
    }

    /* Prompt user for key  */
    printf("Enter the key to search for:\n");
    scanf("%d", &key);

    /* Read key-value pairs from the device  */
    ssize_t bytes_read = read(fd, output_buffer, sizeof(output_buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from device");
        close(fd);
        return EXIT_FAILURE;
    }

    output_buffer[bytes_read] = '\0'; /* Null-terminate the received string  */

    /* Search for the key in the output  */
    char *token = strtok(output_buffer, "\n");
    while (token) {
        int k, v;
        if (sscanf(token, "%d:%d", &k, &v) == 2 && k == key) {
            printf("Key: %d, Value: %d\n", k, v);
            close(fd);
            return EXIT_SUCCESS;
        }
        token = strtok(NULL, "\n");
    }

    printf("Key %d not found.\n", key);
    close(fd);
    return EXIT_SUCCESS;
}

