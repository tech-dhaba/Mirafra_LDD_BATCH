#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <fcntl.h>

#define PATH "/dev/simple_device"
#define SIZE 1024

int main()
{
	char buff[SIZE]="MIRAFRA TECHNOLOGIES";

	int fd = open(PATH,O_WRONLY);
	if(fd==-1)
	{
		perror("open failed");
		exit(0);
	}

	int n = write(fd,buff,SIZE);
	buff[n]='\0';
	printf("successfully written\n");

	return 0;

}

