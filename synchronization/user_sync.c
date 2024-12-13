#include<stdio.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
void main()
{
	int fd=open("/dev/sync_driver",O_RDONLY);
	char s[10];
	read(fd,s,sizeof(s));
	printf("data=%s\n",s);
	sleep(30);

}

