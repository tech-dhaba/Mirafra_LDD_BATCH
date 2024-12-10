#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>

int main()
{
	int fd,fd1;
	char buff[20]="hello world";
	fd=open("hai.txt",O_CREAT |O_RDWR,666);
	if(fd<0)
	{
		printf("open fail");
		return 1;
	}
	printf("%d",fd);

	fd1=write(fd,buff,strlen(buff));
	if(fd1<0)
	{
		printf("write fail");
		return 1;
	}
	write(1,buff,strlen(buff));
	return 0;
}
