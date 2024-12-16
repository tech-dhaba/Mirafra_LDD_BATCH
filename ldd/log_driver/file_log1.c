#include<stdio.h>
#include<stdlib.h>
#include"logger_file.h"
int main()
{
	char message[LOG_MSG_SIZE];
	printf("hello world\n");
	write_log("calling the function main \n");
	while(count > 0)
	{
		if(read_log(message,sizeof(message))==0)
		{
			printf("%s",message);
		}
	}
}

