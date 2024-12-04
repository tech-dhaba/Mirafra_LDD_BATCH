#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FILE_NAME "example.txt"
#define BUFFER_SIZE 100

int main() {
 
    char data[] = "Hello, this is a test data for writing and reading a file in C.";

      int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Error opening file for writing");
        return 1;
    }
    
    ssize_t bytes_written = write(fd, data, strlen(data));
    if (bytes_written == -1) {
        perror("Error writing to file");
        close(fd);
        return 1;
    }
    printf("Data written to file successfully!\n");

  
    close(fd);

    fd = open(FILE_NAME, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file for reading");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) );
    if (bytes_read == -1) {
        perror("Error reading from file");
        close(fd);
        return 1;
    }

      buffer[bytes_read] = '\0';
    printf("Data read from file: %s\n", buffer);

  
    close(fd);

    return 0;
}


