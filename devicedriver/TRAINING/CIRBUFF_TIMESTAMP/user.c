#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_PATH "/dev/keyv"
#define BUFFER_SIZE 256
struct kv_pair {
    //int key;
    char value[BUFFER_SIZE];
};

int main() {
    int fd;
    struct kv_pair kv;
    char buffer[100];
    int i = 0;  // Initialize index to 0

    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    while (1) {
        printf("Enter value (value): ");
        scanf("%255s", kv.value);  // Safe input reading (limiting string size)

        ssize_t bytes_written = write(fd, &kv, sizeof(struct kv_pair));
        if (bytes_written < 0) {
            perror("Failed to write to device");
            close(fd);
            return 1;
        }

        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read < 0) {
            perror("Failed to read from device");
            close(fd);
            return 1;
        }

        buffer[bytes_read] = '\0';  // Null-terminate the string
        printf("Read from device: %s\n", buffer);

        sleep(2);
    }

    close(fd);
    return 0;
}

