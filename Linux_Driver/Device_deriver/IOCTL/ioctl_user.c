/*
 * NAME : PODDUTURI MANASA REDDY
 * KM_ID : KM54BESD03
 * DATE :
 * FILE_NAME :
 * SAMPLE_INPUT :
 * SAMPLE_OUTPUT :
 *
 * */


#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define DEVICE "/dev/ioctl_device"

// IOCTL Commands
#define IOCTL_GET_DATA _IOR('i', 1, int)
#define IOCTL_SET_DATA _IOW('i', 2, int)

int main()
{
    int fd, result;
    int data;

    fd = open(DEVICE, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return -1;
    }

    // Set data using IOCTL
    data = 42;
    result = ioctl(fd, IOCTL_SET_DATA, &data);
    if (result < 0) {
        perror("Failed to set data");
        close(fd);
        return -1;
    }
    printf("Data sent to kernel: %d\n", data);

    // Get data using IOCTL
    result = ioctl(fd, IOCTL_GET_DATA, &data);
    if (result < 0) {
        perror("Failed to get data");
        close(fd);
        return -1;
    }
    printf("Data received from kernel: %d\n", data);

    close(fd);
    return 0;
}
