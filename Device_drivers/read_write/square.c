#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

#define DEVICE_PATH "/dev/char_driver"
int main()
{
	int fd,fd1,fd2;
	char buff[20];
	fd=open(DEVICE_PATH,O_RDWR);
	if(fd<0)
	{
		printf("open fail");
		return 1;
	}
	char mes[20]="10";
	fd1=write(fd,mes,strlen(mes));
	if(fd1<0)
	{
		printf("write fail");
		return 1;
	}
	fd2=read(fd,buff,sizeof(buff)-1);
	if(fd2<0)
	{
		printf("read fail");
		return 1;
	}
	buff[fd2]='\0';
	printf("%s",buff);
	return 0;
}
	

