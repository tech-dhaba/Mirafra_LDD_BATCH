
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
#include<stdlib.h>

#define DEVICE_PATH "/dev/cal"

int main() {
    static char num[10],doublenum[10];
	printf("enter the num:\n");
	scanf("%[^\n]s",num);
    int fd = open(DEVICE_PATH,O_RDWR);

    if (fd < 0) 
	{
        perror("Failed to open device");
        return 1;
    }
	  // Write the integer to the device
    ssize_t w = write(fd,num,strlen(num));
    if (w == -1) 
	{
        perror("Write failed");
        close(fd);
        return 1;
    }
	ssize_t r = read(fd,doublenum,sizeof(doublenum)-1);
    if (r == -1) 
	{
        perror("Read failed");
        close(fd);
        return 1;
    }
	int res=atoi(doublenum);
    printf("Read doubled number from the device: %d\n",res);
    close(fd);
    return 0;
}
