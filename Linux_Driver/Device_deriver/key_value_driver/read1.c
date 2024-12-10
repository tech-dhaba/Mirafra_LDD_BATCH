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

#define DEVICE_PATH "/dev/k_vall"

struct key {
    char key[64];
    char value[256];
};

int i = 0;

int main() {
    char k_buffer[1024];  // Buffer to hold the read data from the device
    struct key max_key[10];
	char k[64];
    int fd = open(DEVICE_PATH, O_RDWR);

    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    // Main input loop to write key-value pairs
    while (1) {
        printf("Enter the key and value:\n");

        // Read key and value from user input
        scanf("%63s %255s", max_key[i].key, max_key[i].value);  // Read value and key
		// Write the data to the device
        int x = write(fd,&max_key[i],sizeof(struct key));
        if (x == -1) {
            printf("Write failed\n");
            return 1;
        }i++;

        // Reading from the device
        int n = read(fd, k_buffer, sizeof(k_buffer) - 1);  // Leave space for null-terminator
        if (n < 0) {
            perror("Read failed");
            return 1;
        }

        k_buffer[n] = '\0';  // Null-terminate the buffer

        printf("Device says: %s\n", k_buffer);

        // Check if the message from the device indicates limit exceeded
        if (strcmp(k_buffer, "limit exceed") == 0) {
            printf("Limit exceeded. Exiting.\n");
            break;
        }
    }
	printf("Enter key to search: ");
    scanf("%63s", k);
    if (write(fd, k, strlen(k) + 1) == -1) {
        perror("Key write failed");
        return 1;
    }
    if (read(fd,k_buffer, sizeof(k_buffer) - 1) == -1) {
        perror("Read failed");
        return 1;
    }
   k_buffer[1023] = '\0';
    printf("Search result: %s\n",k_buffer);

    close(fd);
    return 0;
}



