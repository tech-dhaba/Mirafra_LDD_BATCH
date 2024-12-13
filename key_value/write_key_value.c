#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#define  PATH "/dev/key-value"
#define MAX 10
struct dictionary
{
	char key[20];
	char value[20];
};

int main()
{
	int n;struct dictionary dict[10];
	int fd = open(PATH,O_RDWR);
	char buff[200];
	printf("Enter the no of key values u want u store\n");
	scanf("%d",&n);
	
	for(int i=0 ;i<n&&MAX;i++)
	{
		printf("Enter the key %d:",i+1);
		scanf("%s",dict[i].key); 
//		write(fd,dict[i].key,strlen(dict[i].key));
		printf("Enter the value for key %d:",i+1);
		scanf("%s",dict[i].value);
		sprintf(buff,"%s %s",dict[i].key,dict[i].value);
		write(fd,buff,strlen(buff));
	}
	lseek(fd,0,SEEK_SET);

	n=read(fd,buff,200);
	buff[n]='\0';
	printf("%s",buff); 
	close(fd);
	
	return 0;
}

