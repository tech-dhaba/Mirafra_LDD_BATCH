#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main() {
    int choice, fd1;
    char message[100]; 
    FILE *fp;
    printf("1: System calls, 2: Library APIs\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    printf("Enter the message:");
    scanf(" %[^\n]s", message);
    switch (choice) {
        case 1:
            // Using system calls
            fd1 = open("hello.txt", O_CREAT | O_WRONLY | O_APPEND, 0666);
            if (fd1 < 0) {
                perror("open failed");
                exit(1);
            }
            if (write(fd1, message, strlen(message)) < 0) {
                perror("write failed");
                close(fd1);
                exit(1);
            }
            printf("Message using system calls.\n");
            break;
        case 2:
            // Using library APIs
            fp = fopen("hello.txt", "a+"); // Open in append mode
            if (fp == NULL) {
                perror("fopen failed");
                exit(1);
            }
            fprintf(fp, "%s", message); // Write the message
            fclose(fp);
            printf("Message logged using library APIs.\n");
            break;
    default:
            printf("Invalid choice!\n");
            break;
    }
    return 0;
}
