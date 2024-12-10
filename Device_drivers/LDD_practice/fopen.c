#include<string.h>
#include<stdio.h>
int main()
{
	FILE *fd;
	char ch[100];
		fd=fopen("hello.txt","w");
	if(fd==NULL)
	{
		printf("open fail");
		return 1;
	}
	printf("writing the data into file\n");
	fprintf(fd,"welcome to mirafra technologys");
	fclose(fd);
		fd=fopen("hello.txt","r");
		if(fd==NULL)
		{
			printf("open fail");
			return 1;
		}
		while(fgets(ch,sizeof(ch),fd)!=NULL)
		{
		printf("%s",ch);
		}
	fclose(fd);
}

