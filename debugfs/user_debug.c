#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#define PATH "/sys/kernel/debug/debugfs_example/value"
void main(int argc,char **argv)
{
	int fd=open(PATH,O_WRONLY);
//	int b=0;
	char a=argv[1][0],b;
//	char s[10];
	write(fd,&a,sizeof(a));
	close(fd);
	open(PATH,O_RDONLY);
	read(fd,&b,sizeof(b));
	printf("data=%c\n",b);

}

