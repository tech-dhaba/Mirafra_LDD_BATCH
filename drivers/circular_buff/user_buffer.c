#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

struct logs {
    char msg[100];
};

int main() {
    struct logs svar[5];
    int fd = open("/dev/simple_device1", O_RDWR);
    if (fd == -1) {
        printf("Open failed\n");
        return -1;
    }
    
    int i = 0;
    char str[100], res[500];
    while (1) {
        printf("Enter msg: ");
        scanf(" %[^\n]s", svar[i].msg);
        
        // Write to device
        sprintf(str, "%s", svar[i].msg);
        int w = write(fd, str, strlen(str));
        if (w == -1)
            printf("Write failed\n");

        // Read from device (get all stored messages)
        int r = read(fd, res, sizeof(res) - 1);
        if (r == -1)
            printf("Read failed\n");
        else {
            res[r] = '\0'; // Null-terminate the response string
            printf("Logs from kernel:%s", res);
        }

        i = (i + 1) % 5; // Cycle through the buffer
        sleep(2);  // Delay to give time for device interaction
    }

    close(fd);
    return 0;
}

