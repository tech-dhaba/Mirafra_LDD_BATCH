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
	static int num;
	printf("enter the num:\n");
	scanf("%d",&num);
	static int k_num;
	int fd=open("/dev/simple_driver", O_RDWR);
	if (fd == -1)
	{
        printf("open failed");
        return 1;
    }
	ssize_t w = write(fd,&num,sizeof(int));
    if (w == -1) 
	{
        printf("write failed");
        return 1;
    }

    printf("Written %ld bytes to the device\n", w);
	ssize_t r= read(fd,&k_num,sizeof(int));
    if (r == -1) 
	{
        printf("Failed to read");
        return 1;
    }
	k_num*=2;
	w=write(fd,&k_num,sizeof(int));
 	if (w == -1) 
	{
        printf("Failed to read");
        return 1;
    }

	int fd1,num1;
    fd1 = open("/dev/simple_driver", O_RDONLY);
    if (fd1 == -1) 
	{
        perror(" open failed");
        return 1;
    }
	r=read(fd1,&num1,sizeof(int));
	if (r == -1) 
	{
        printf("Failed to read");
        return 1;
    }
/*
	w=write(1,&num1,sizeof(int));
	 if (w == -1)
	{
        printf("Failed to read");
        return 1;
    }
*/
    printf("Read %ld bytes from the device: %d\n", r,num1);
   close(fd);
    close(fd1);
	return 0;
}


