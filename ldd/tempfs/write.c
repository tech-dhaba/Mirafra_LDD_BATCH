#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
int main()
{
	int ret,fd;
	fd=open("/tmp/tmpfs_example_file",O_WRONLY);
	if(fd==-1)
	{
		perror("failed to open the file \n");
		exit(EXIT_FAILURE);
	}
	char buf[100];
	strcpy(buf,"yaswanth kumar reddy");
	ret=write(fd,buf,sizeof(buf));
	if(ret==-1)
	{
		perror("faield to read from the file \n");
		exit(EXIT_FAILURE);
	}
}
