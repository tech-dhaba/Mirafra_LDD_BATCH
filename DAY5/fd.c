#include<stdio.h>
#include<fcntl.h>
int main()
{
	int fd=open("demo.txt",O_WRONLY);
	if(fd<0)
	{
		perror("open");
		return 0;
	}
	char str[50];
	gets(str);
	write(fd,str,strlen(str));
	write(1,str,strlen(str));
}
