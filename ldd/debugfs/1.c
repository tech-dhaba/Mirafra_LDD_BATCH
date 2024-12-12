#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h>
int main()
{
	int ret,fd;
	fd=open("/sys/kernel/debug/debuggfs/1",O_RDWR ,0666);
	if(fd==-1)
	{
		perror("failed to open the file \n");
		exit(0);
	}
	char buf[100];
	snprintf(buf,sizeof(buf),"yaswanth kumar reddy tiyyagura ");
	ret=write(fd,buf,strlen(buf));
	if(ret==-1)
	{
		perror("failed to write\n");
		exit(0);
	}
	close(fd);
	fd=open("/sys/kernel/debug/debuggfs/1",O_RDWR ,0666);
	if(fd==-1)
	{
		perror("failed to read from the file \n");
		exit(0);
	}
	ret=read(fd,buf,sizeof(buf));
	if(ret==-1)
	{
		perror("failed to read from the file \n");
		exit(0);
	}
	close(fd);
}
