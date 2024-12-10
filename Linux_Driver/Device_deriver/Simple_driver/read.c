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

int main() {
    int fd;
    char buffer[1024];
    fd = open("/dev/simple_driver", O_RDONLY);
    if (fd == -1) 
	{
        perror(" open failed");
        return 1;
    }

    ssize_t r= read(fd, buffer, sizeof(buffer) - 1);
    if (r == -1) 
	{
        printf("Failed to read");
        return 1;
    }
    buffer[r] = '\0';

    printf("Read %ld bytes from the device: %s\n", r, buffer);
    close(fd);
    return 0;
}

