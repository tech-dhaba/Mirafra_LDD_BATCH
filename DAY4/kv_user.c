#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define DEVICE "/dev/KV_pair"
#define MAX_KEY_LEN 32
#define MAX_VAL_LEN 128

void write_kv(const char *key, const char *value) {
    char buffer[MAX_KEY_LEN + MAX_VAL_LEN + 2];
    int fd = open(DEVICE, O_WRONLY);
    if (fd < 0) {
        perror("Failed to open device");
        return;
    }

    snprintf(buffer, sizeof(buffer), "%s,%s", key, value);
    if (write(fd, buffer, strlen(buffer)) < 0) {
        perror("Failed to write to device");
    } else {
        printf("Written: %s=%s\n", key, value);
    }

    close(fd);
}

void read_kv(const char *key) {
    char buffer[MAX_VAL_LEN];
    int fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return;
    }

    if (write(fd, key, strlen(key)) < 0) {
        perror("Failed to send key to device");
    } else {
        if (read(fd, buffer, sizeof(buffer)) < 0) {
            perror("Failed to read from device");
        } else {
            printf("Read: %s=%s\n", key, buffer);
        }
    }

    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s write <key> <value> | read <key>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "write") == 0 && argc == 4) {
        write_kv(argv[2], argv[3]);
    } else if (strcmp(argv[1], "read") == 0 && argc == 3) {
        read_kv(argv[2]);
    } else {
        printf("Invalid command. Usage: %s write <key> <value> | read <key>\n", argv[0]);
        return 1;
    }

    return 0;
}
