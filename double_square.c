#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
void main()
{
	int fd=open("/dev/simple",O_WRONLY,0664);
	if(fd<0)
	{
		perror("open");
		return;
	}
	char num[30];
	printf("Enter the number to double\n");
	scanf("%s",num);
	write(fd,num,strlen(num)+1);
	close(fd);
//	sleep(4);
	char res[30];
	int fd1=open("/dev/simple",O_RDONLY,0664);
	read(fd1,res,sizeof(res));
	//res[size]='\0';
	int res1=atoi(res);
	printf("res1=%d",res1);

//int res=double_num(&num);

}
