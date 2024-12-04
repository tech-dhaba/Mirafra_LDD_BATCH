#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
void main()
{
	char s[50]="sushma Arlingam";
	int fp=open("/dev/simple",O_RDONLY,0664);
	if(fp<0)
	{
		perror("failed to open");
		return;
	}
//write(fp,s,50);
	char p[50];
	int size=read(fp,p,50);
	p[size]='\0';
	printf("output is %s",p);
	close(fp);

}
