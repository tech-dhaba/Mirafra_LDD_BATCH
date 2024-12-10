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
	static int num1,num2;
	printf("enter the 2 num:\n");
	scanf("%d%d",&num1,&num2);
	static int k_num1,k_num2;
	int fd=open("/dev/simple_driver", O_RDWR);
	if (fd == -1)
	{
        printf("open failed");
        return 1;
    }
	ssize_t w1 = write(fd,&num1,sizeof(int));
    if (w1== -1) 
	{
        printf("write failed");
        return 1;
    }
	ssize_t w2= write(fd,&num2,sizeof(int));
    if (w2 == -1) 
	{
        printf("write failed");
        return 1;
    }
	ssize_t r1= read(fd,&k_num1,sizeof(int));
    if (r1== -1) 
	{
        printf("Failed to read");
        return 1;
    }
	ssize_t r2= read(fd,&k_num2,sizeof(int));
    if (r2== -1) 
	{
        printf("Failed to read");
        return 1;
    }

	printf("%d %d\n",k_num1,k_num2);
	/*
//	k_num*=2;
	w1=write(fd,&k_num,sizeof(int));
 	if (w1== -1) 
	{
        printf("Failed to read");
        return 1;
    }

	int fd1,num;
    fd1 = open("/dev/simple_driver", O_RDONLY);
    if (fd1 == -1) 
	{
        perror(" open failed");
        return 1;
    }
	r=read(fd1,&num,sizeof(int));
	if (r == -1) 
	{
        printf("Failed to read");
        return 1;
    }

    printf("Read %ld bytes from the device: %d\n", r,num);
   close(fd);
    close(fd1);*/
	return 0;
}

