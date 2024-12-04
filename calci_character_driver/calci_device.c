#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#define PATH "/dev/simple_device"

int main(int argc,char *argv[])
{
	int fd=open(PATH,O_WRONLY);	
	char buff[20];
	if(fd==-1)
	{
		perror("open failed");
		exit(0);
	}
	
	sprintf(buff,"%s %s %s",argv[1],argv[2],argv[3]);
	printf("%s",buff);
	write(fd,buff,strlen(buff));
	return 0;
	
}
