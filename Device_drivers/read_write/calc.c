#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#define DEVICE_PATH "/dev/calc_driver"
struct calc
{
	int n1;
	int n2;
	char operation;
};
int main(int argc,char *argv[])
{
	int fd,fd1,fd2;
	fd=open(DEVICE_PATH,O_RDWR);
	if(fd<0)
	{
		printf("open fail");
		return 1;
	}

	struct calc data;
		data.n1=atoi(argv[2]);
		data.n2=atoi(argv[3]);
		data.operation=argv[1][0];
	fd1=write(fd,&data,sizeof(data));
	if(fd1<0)
	{
		printf("write fail");
		return 1;
	}
char buff[100]={0};
fd2=read(fd,buff,sizeof(buff)-1);
if(fd2<0)
{
	printf("read fail");
	return 1;
}
printf("%s",buff);
return 0;
}
