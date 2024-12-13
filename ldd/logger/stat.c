#include <stdio.h>
#include <sys/stat.h>
int main()
{
	struct stat buf;

	stat("file.txt",&buf);
	printf("permissions before using chmod %o\n",buf.st_mode & 0777);	
	
	if(chmod("file.txt",0111) == -1)
		printf("Denied to change permissions\n");

	stat("file.txt",&buf);
	printf("permissions after using chmod %o\n",buf.st_mode & 0777);
	
	return 0;
}
