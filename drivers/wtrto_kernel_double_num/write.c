#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_PATH "/dev/simple_driver1"

int main() {
    int num;
    printf("Enter a number: ");
    scanf("%d", &num);

    // Open the device with read-write permissions
    int fd = open(DEVICE_PATH, O_RDWR);  // O_RDWR allows both reading and writing

    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    // Write the number to the device
    ssize_t bytes_write = write(fd, &num, sizeof(int));
    if (bytes_write==-1) {
        perror("Failed to write data to device");
        close(fd);
        return 1;
    }

    int buff;
    // Read the doubled number from the device
    ssize_t bytes_read = read(fd, &buff, sizeof(int));
    if (bytes_read==-1) {
        perror("Failed to read data from device");
        close(fd);
        return 1;
    }

    printf("Data from driver (doubled): %d\n", buff);

    close(fd);
    return 0;
}

