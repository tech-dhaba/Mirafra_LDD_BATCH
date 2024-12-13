#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#define  PATH "/dev/key-value"
#define MAX 10

int main()
{
	int n;char buff[20];
	int fd = open(PATH,O_RDWR);
	printf("Enter the index:");
	scanf("%d",&n);
	sprintf(buff,"key %d",n);
	
	write(fd,buff,strlen(buff));
	n=read(fd,buff,20);
	buff[n]='\0';

	printf("%s",buff);
	close(fd);
	
	return 0;
}

