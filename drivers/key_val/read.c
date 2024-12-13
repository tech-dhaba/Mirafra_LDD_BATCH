#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>

int main()
{
	int fd=open("/dev/suji",O_RDWR);
	if(fd==-1)
		printf("read failed\n");
	int n;
	char ch[20],res[20];
	printf("enter index:");
	scanf("%d",&n);
	sprintf(ch,"%d",n);
	int w=write(fd,ch,strlen(ch));
	if(w==-1)
		printf("write failed\n");
	int r=read(fd,res,20);
	if(r==-1)
		printf("read failed\n");
	res[r]='\0';
	printf("%s\n",res);
	close(fd);
	return 0;
}
