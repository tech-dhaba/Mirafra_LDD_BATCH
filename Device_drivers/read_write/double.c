#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<error.h>
#define DEVICE_PATH "/dev/char_driver"
int main()
{

	int fd,fd1,fd2;
	char mes[20];
	fd=open(DEVICE_PATH,O_RDWR);
	if(fd<0)
	{
		perror("open fail");
		return 1;
	}
   char buff[10]="10";
	write(fd,buff,strlen(buff));
	if(fd1<0)
	{
		printf("write fail");
		return 1;
	}
fd2=read(fd,mes,sizeof(mes)-1);
if(fd2<0)
{
	printf("read fail");
	return 1;
}
mes[fd2]='\0';
printf("%s",mes);
}
