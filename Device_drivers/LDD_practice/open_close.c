#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<fcntl.h>
int main()
{
	int fd,fd1,fd2;
	char buff[10]="balasai";
	char arr[20];
	fd=open("abc.txt",O_CREAT|O_RDWR);
	if(fd<0)
	{
		printf("open fail");
		exit(1);
	}
		fd1=write(fd,buff,strlen(buff));
	if(fd1<0)
	{
		printf("write fail");
		exit(1);
	}
	printf("writte data:%s\n",buff);
	lseek(fd,0,SEEK_SET);
	fd2=read(fd,&arr,strlen(arr));
	if(fd2<0)
	{
		printf("read fail");
		exit(1);
	}
	arr[fd2]='\0';
	printf("read data:%s\n",arr);
	close(fd);
	return 0;
}

