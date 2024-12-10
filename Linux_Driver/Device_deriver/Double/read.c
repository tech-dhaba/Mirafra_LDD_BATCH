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
#include <unistd.h>
#include <string.h>

#define DEVICE_PATH "/dev/driver_double"

int main() {
    static int num,double_num;
	printf("enter the num:\n");
	scanf("%d",&num);
    int fd = open(DEVICE_PATH,O_RDWR);

    if (fd < 0) 
	{
        perror("Failed to open device");
        return 1;
    }
	  // Write the integer to the device
    ssize_t w = write(fd,&num,sizeof(int));
    if (w == -1) 
	{
        perror("Write failed");
        close(fd);
        return 1;
    }
	ssize_t r = read(fd,&double_num,sizeof(int));
    if (r == -1) 
	{
        perror("Read failed");
        close(fd);
        return 1;
    }
    printf("Read doubled number from the device: %d\n", double_num);
    close(fd);
    return 0;
}
