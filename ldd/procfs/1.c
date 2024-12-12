#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
int main()
{
	int ret,fd;
	fd=open("/proc/my_proc_file", O_RDWR);
	if(fd==-1)
	{
		perror("failed to open the file \n");
		exit(1);
	}
	char buf[20];
	int n=sizeof(buf);
	ret=read(fd,buf,n-1);
	if(ret==-1)
	{
		perror("failed to read from the file \n");
		exit(1);
	}
	buf[n]='\0';
	printf("the message inside the buffer is :%s\n",buf);
	close(fd);
}
