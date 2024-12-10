#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#define DEVICE_PATH "/dev/suji"

int main() {
    int fd;
    char buffer[1024];
    char key[50];
    int value;
    ssize_t bytes_read;

    // Open the device file
    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return 1;
    }

    // Prompt user to input key-value pairs
//    for (int i = 0; i < 6; i++) {  // Allowing 6 entries (5 normal, 1 to trigger limit)
  while(1){
    printf("Enter key (string): ");
        scanf("%s", key);
        printf("Enter value (integer): ");
        scanf("%d", &value);

        // Construct the input data in the format "key value"
        snprintf(buffer, sizeof(buffer), "%s %d", key, value);

        // Writing data to the device
        ssize_t bytes_written = write(fd, buffer, strlen(buffer));
        if (bytes_written < 0) {
            perror("Failed to write to the device");
            close(fd);
            return 1;
        }
	bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read the device");
        close(fd);
        return 1;
    }
    buffer[bytes_read] = '\0';
    if(strcmp(buffer,"limit exceed")==0)
	    break;
    }
/*
    // Read the data back from the device
    printf("Reading data from device...\n");
    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read the device");
        close(fd);
        return 1;
    }
    buffer[bytes_read] = '\0';  // Null-terminate the string
*/
    printf("Read from device:\n%s\n", buffer);

    // Close the device file
    close(fd);

    return 0;
}

