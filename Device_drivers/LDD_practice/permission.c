#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
int main()
{
	struct stat a;
	if((stat("abc.txt",&a))<0)
	{
		perror("stat fail");
		exit(1);
	}
	printf("%o\n",a.st_mode & 0777);
	getchar();
	if((chmod("abc.txt",0111))<0)
	{
		perror("chmod fail");
		exit(1);
	}
	stat("abc.txt",&a);
	printf("%o",a.st_mode &0777);
	return 0;
}

