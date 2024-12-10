#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
int main()
{
	int fd,fd1,fd2;
	char buff[10]="balasai";
	fd=open("abc.txt",O_CREAT|O_WRONLY);
	if(fd<0)
	{
		printf("open fail");
		exit(1);
	}
		/*fd2=write(fd,buff,strlen(buff));
	if(fd2<0)
	{
		printf("write fail");
		exit(1);
	}*/
	write(1,buff,strlen(buff));
	return 0;
}

