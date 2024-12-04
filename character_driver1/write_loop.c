#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
//	int fd = open("text.txt",O_RDWR|O_CREAT,0666);
	int fd = open("/dev/simple_device",O_RDWR);
	int i;char buff[30];
	for(i=0;i<5;i++)
		write(fd,"hello ",6);
	close(fd);
	return 0;
}

