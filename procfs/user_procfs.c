#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
void main()
{
	int fd;
	fd=open("/proc/my_proc_file",O_WRONLY);
	char s[20];
	scanf("%s",s);
	write(fd,s,strlen(s)+1);
	close(fd);

	
	fd=open("/proc/my_proc_file",O_RDONLY);
	char m[20];
	read(fd,m,sizeof(m));
	printf("changing==%s\n",m);
}
