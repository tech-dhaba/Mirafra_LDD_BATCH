#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<stdio.h>

#define DEVICE_PATH "/dev/kv_store"

struct key_value
{
	char key[128];
	char value[128];
};
int i=0;
int main()
{
	char buff[128];
	struct key_value data[4];
	int fd,fd1,fd2,ch;
	fd=open(DEVICE_PATH,O_RDWR);
	if(fd<0)
	{
		printf("open fail");
		return 1;
	}
	while(1)
	{
		printf("Enter the key and value\n");
		scanf("%19s%19s",data[i].key,data[i].value);
		fd1=write(fd,&data[i],sizeof(data));
		if(fd1<0)
		{
			printf("write fail");
			return 0;
		}
		i++;
		printf("receive values\n");
		fd2=read(fd,buff,sizeof(buff)-1);
		if(fd2<0)
		{
			printf("read fail");
			return 1;
		}
			buff[fd2]='\0';
			if((strcmp(buff,"limit exceed"))==0)
			{
				printf("limit:%s\n",buff);
				break;
			}
			printf("key & value:%s",buff);
	}
	char key[128];
	printf("enter the key\n");
	scanf(" %19s",key);
	fd1=write(fd,key,strlen(key)+1);
	if(fd1<0)
	{
		printf("write fail");
		return 0;
	}
	char buff1[128];
	printf("receive value:");
	fd2=read(fd,buff1,sizeof(buff1)-1);
	if(fd2<0)
	{
		printf("read fail");
		return 1;
	}
	buff1[fd2]='\0';
	printf("%s",buff1);
}
