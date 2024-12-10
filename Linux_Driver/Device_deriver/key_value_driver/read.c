/*
 * NAME : PODDUTURI MANASA REDDY
 * KM_ID : KM54BESD03
 * DATE :
 * FILE_NAME :
 * SAMPLE_INPUT :
 * SAMPLE_OUTPUT :
 *
 * */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_PATH "/dev/key_value_driver"
struct key
{
	char key[64];
	char value[256];
};
int i=0;
int main() {
	char k_buffer[1024];
	struct key max_key[10];
	int fd = open(DEVICE_PATH, O_RDWR);

	if (fd < 0) {
		perror("Failed to open device");
		return 1;
	}
	printf("enter the key and value:\n");
	scanf("%[^\n]s%[^\n]s",max_key[i].key,max_key[i].value);
	char buffer[322]; // key + ',' + value + '\0'

	// Prepare the key-value pair in the format key,value
	sprintf(buffer,"%s %s", max_key[i].key, max_key[i].value);
	i++;
	int x=write(fd,buffer,strlen(buffer));
	if(x==-1)
	{
		printf("write fail:\n");
	}
	while(1)
	{
		int n=read(fd,k_buffer,sizeof(k_buffer));
		printf("%s\n",k_buffer);
		if(strcmp(k_buffer,"limit exceed")==0)
		{
			break;
		}
		else
		{
			printf("enter the key and value:\n");
			scanf(" %[^\n]s%[^\n]s",max_key[i].key,max_key[i].value);
			sprintf(buffer,"%s %s", max_key[i].key, max_key[i].value);
			int x=write(fd,buffer,strlen(buffer));
			if(x==-1)
			{
				printf("write fail:\n");
				return 1;
			}
			i++;
		}
	}


	close(fd);
	return 0;
}
