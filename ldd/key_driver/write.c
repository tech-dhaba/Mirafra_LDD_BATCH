#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_FILE "/dev/key_value_driver"

int main() {
    int fd;
    char input_buffer[256];

    /* Open the device file  */
    fd = open(DEVICE_FILE, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open device");
        return EXIT_FAILURE;
    }

    /* Prepare key-value pairs */
    printf("Enter key-value pairs in the format key:value (space-separated):\n");
    fgets(input_buffer, sizeof(input_buffer), stdin);
    input_buffer[strcspn(input_buffer, "\n")] = '\0'; // Remove newline character

    /* Write key-value pairs to the device  */
    if (write(fd, input_buffer, strlen(input_buffer)) < 0) {
        perror("Failed to write to device");
        close(fd);
        return EXIT_FAILURE;
    }

    printf("Successfully sent key-value pairs to the kernel.\n");
    close(fd);
    return EXIT_SUCCESS;
}

