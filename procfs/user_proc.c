#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#define PATH "/proc/my_proc_file"
void main(int argc,char **argv)
{
	int fd=open(PATH,O_WRONLY);
	char s[50];
	write(fd,argv[1],strlen(argv[1])+1);
	close(fd);
	open(PATH,O_RDONLY);
	read(fd,s,sizeof(s));
	printf("data=%s\n",s);

}

