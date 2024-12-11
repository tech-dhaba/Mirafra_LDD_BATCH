#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define DEVICE "/dev/ioctl_driver"
#define IOCTL_MAGIC 'x'
#define IOCTL_SET_STRING _IOW(IOCTL_MAGIC, 1, char *)
#define IOCTL_GET_STRING _IOR(IOCTL_MAGIC, 2, char *)

int main() {
    int fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    char write_buffer[] = "Hello, Kernel via IOCTL!";
    ioctl(fd, IOCTL_SET_STRING, write_buffer);

    char read_buffer[100];
    ioctl(fd, IOCTL_GET_STRING, read_buffer);
    printf("Received from driver: %s\n", read_buffer);

    close(fd);
    return 0;
}

