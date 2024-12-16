#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
void main(int argc,char **argv)
{
	int fd;
	fd=open("/sys/kernel/debug/debugfs_pramod/value",O_WRONLY);
	char a=argv[1][0],b;
	write(fd,&a,1);
	close(fd);

	
	fd=open("/sys/kernel/debug/debugfs_pramod/value",O_RDONLY);
	read(fd,&b,1);
	printf("data==%c\n",b);
	close(fd);

}
