#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVICE "/dev/rw_driver"

int main() {
    int fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    char write_buffer[] = "Hello, Kernel!";
    write(fd, write_buffer, sizeof(write_buffer));

    char read_buffer[100];
    read(fd, read_buffer, sizeof(read_buffer));
    printf("Received from driver: %s\n", read_buffer);

    close(fd);
    return 0;
}

