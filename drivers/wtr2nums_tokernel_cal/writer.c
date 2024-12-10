#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/types.h>

int main()
{
	int fd=open("/dev/cal",O_RDWR);
	if(fd==-1)
		printf("failed to open file\n");
	char buff[100];
	int num1,num2;
	char str[10];
	printf("enter num1 and num2:");
	scanf("%d %d",&num1,&num2);
	printf("enter msg:");
	scanf(" %s",str);
	sprintf(buff,"%d %d %s",num1,num2,str);
	int r=write(fd,buff,strlen(buff));
	if(r==-1)
		printf("write failed \n");
	char result[50];
	int rd=read(fd,result,50);
	if(rd==-1)
		printf("read failed\n");
	printf("%s\n",result);
	close(fd);
	return 0;
}
