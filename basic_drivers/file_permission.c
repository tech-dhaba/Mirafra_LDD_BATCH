#include<stdio.h>
#include<sys/stat.h>
int main(int argc,char **argv)
{
	if(argc!=2)
	{
		printf("usage:./a.out filename\n");
		return 0;
	}
	struct stat v;
	if(lstat(argv[1],&v)<0)
	{
		perror("stat");
		return 0;
	}
	printf("%o\n",v.st_mode&0777);
	chmod(argv[1],0611);
	lstat(argv[1],&v);
	printf("%o\n",v.st_mode&0777);


}
