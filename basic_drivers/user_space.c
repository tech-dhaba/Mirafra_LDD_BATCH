#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
void main(int argc,char **argv)
{
	int fd=open(argv[2],O_WRONLY);
	int in_num,out_num;
	char s[20],d[20];
//	strcpy(s,argv[1]);
//	int l=strlen(s);
//	s[l]='\0';
	write(fd,argv[1],strlen(argv[1])+1);
	close(fd);
	fd=open(argv[2],O_RDONLY);
	read(fd,d,sizeof(d));
	out_num=atoi(d);
	printf("%d\n",out_num);
	close(fd);

}
