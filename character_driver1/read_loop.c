#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
int main()
{
	char buff[100];
	int fd = open("/dev/simple_device",O_RDONLY);
	int n= read(fd,buff,20);
	buff[n]='\0';
	printf("%d %s",n,buff);
	 n= read(fd,buff,20);
	buff[n]='\0';
	printf("%d %s",n,buff);
	return 0;
}

