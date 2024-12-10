#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>

struct abc
{
	char key[20];
	char val[20];
};

int main()
{
	struct abc s[10];
	char buff[200],result[100];
	int i=0;
	int fd=open("/dev/key",O_RDWR);
	if(fd==-1)
		printf("failed to open file \n");
	while(i<5)
	{
		//sleep(1);
		printf("enter key and value:");
		scanf("%s %s",s[i].key,s[i].val);
		sprintf(buff,"%s %s",s[i].key,s[i].val);
		int w=write(fd,buff,strlen(buff));
		if(w==-1)
			printf("write failed\n");
		i++;
	//	int r=read(fd,result,strlen(result)+1);
	//	if(r==-1)
	//		printf("read failed\n");
	//	printf("%s\n",result);
	//	if(strcmp(result,"limit exceed")==0)
	//		break;
	}
	close(fd);
	return 0;
}
