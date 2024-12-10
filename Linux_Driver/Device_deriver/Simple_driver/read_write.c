/*
 * NAME : PODDUTURI MANASA REDDY
 * KM_ID : KM54BESD03
 * DATE :
 * FILE_NAME :
 * SAMPLE_INPUT :
 * SAMPLE_OUTPUT :
 *
 * */
#include<string.h>
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
int main()
{
	char *buff="hello";
	int fd=open("/dev/simple_driver", O_WRONLY);
	if (fd == -1)
	{
        printf("open failed");
        return 1;
    }
	ssize_t w = write(fd,buff,strlen(buff));
    if (w == -1) 
	{
        printf("write failed");
        return 1;
    }

    printf("Written %ld bytes to the device\n", w);
    close(fd);
	return 0;
}

