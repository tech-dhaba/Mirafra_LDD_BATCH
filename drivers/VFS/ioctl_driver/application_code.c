#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>

// Define the ioctl commands
#define IOCTL_SET_VALUE _IOW('M', 0, int)
#define IOCTL_GET_VALUE _IOR('M', 1, int)

int main()
{
    int fd;
    int value;

    // Open the device file
    fd = open("/dev/simple_device", O_RDWR);
    if (fd == -1) {
        perror("Failed to open the device");
        return -1;
    }

    // Set the value to the kernel (IOCTL_SET_VALUE)
    value = 42;
    int res;
    if (ioctl(fd, IOCTL_SET_VALUE, &value) == -1) {
        perror("Failed to set value via ioctl");
        close(fd);
        return -1;
    }

    // Get the value from the kernel (IOCTL_GET_VALUE)
    if (ioctl(fd, IOCTL_GET_VALUE, &res) == -1) {
        perror("Failed to get value via ioctl");
        close(fd);
        return -1;
    }

    printf("Value from kernel: %d\n", res);

    // Close the device file
    close(fd);
    return 0;
}

