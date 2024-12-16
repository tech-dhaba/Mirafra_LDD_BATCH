#include <arpa/inet.h>  // For socket-related functions
#include <stdlib.h>     // For exit(), EXIT_SUCCESS, etc.
#include <stdio.h>      // For printf(), perror(), fgets(), etc.
#include <string.h>     // For strlen(), memset(), etc.
#include <unistd.h>     // For close(), read(), sleep(), etc.

#define port 6543
#define buffer_size 1024

int main() {
    int sock = 0;
    struct sockaddr_in ser_addr;
    char message[100];
    char buffer[buffer_size] = {0};

    // Create a socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Can't create the endpoint\n");
        exit(EXIT_FAILURE);
    }

    // Setup the server address structure
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(port);

    // Convert IP address from text to binary
    if (inet_pton(AF_INET, "127.0.0.1", &ser_addr.sin_addr) <= 0) {
        perror("Failed to convert IP address\n");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&ser_addr, sizeof(ser_addr)) < 0) {
        perror("Failed to connect\n");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // Communication loop
    while (1) {
        printf("Enter data to send:\n");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = 0; // Remove newline character

        send(sock, message, strlen(message), 0);
        printf("Message sent: %s\n", message);

        int valread = read(sock, buffer, buffer_size);
        if (valread > 0) {
            printf("Server response: %s\n", buffer);
            memset(buffer, 0, buffer_size);
        }
        sleep(1);
    }

    close(sock);
    return 0;
}

