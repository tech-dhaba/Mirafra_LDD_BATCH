#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/wait.h>

#define UART_DEVICE "/dev/tty" // Update to use /dev/ttyS0

void parent_process(int uart_fd) {
    char user_input[256]; // Buffer to hold user input

    while (1) {
        // Wait for the parent to enter some data
        printf("Parent: Please enter a message to send: ");
        if (fgets(user_input, sizeof(user_input), stdin) == NULL) {
            perror("Error reading input");
            close(uart_fd);
            exit(-1);
        }

        // Remove newline character from input
        user_input[strcspn(user_input, "\n")] = '\0';

        // Write the message to UART
        int bytes_written = write(uart_fd, user_input, strlen(user_input));
        if (bytes_written < 0) {
            perror("UART write failed");
            close(uart_fd);
            exit(-1);
        }
        printf("Parent: Sent: %s\n", user_input);
        
        sleep(1);  // Simulate delay between writes (optional)
    }
}

void child_process(int uart_fd) {
    char read_buffer[256]; // Buffer to store received data
    memset(read_buffer, 0, sizeof(read_buffer));

    while (1) {
        // Read data from UART
        int bytes_read = read(uart_fd, read_buffer, sizeof(read_buffer) - 1);
        if (bytes_read < 0) {
            perror("UART read failed");
            close(uart_fd);
            exit(-1);
        }

        if (bytes_read > 0) {
            read_buffer[bytes_read] = '\0';  // Null-terminate the string
            printf("Child: Received: %s\n", read_buffer);
        }
        usleep(500000);  // Simulate delay between reads (optional)
    }
}

int main() {
    // Open the UART device
    int uart_fd = open(UART_DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart_fd == -1) {
        perror("Unable to open UART");
        return -1;
    }

    // Configure UART
    struct termios options;
    tcgetattr(uart_fd, &options);       // Get current UART settings
    cfsetispeed(&options, B9600);      // Set input baud rate
    cfsetospeed(&options, B9600);      // Set output baud rate
    options.c_cflag |= (CLOCAL | CREAD); // Enable receiver and set local mode
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;            // 8 data bits
    options.c_cflag &= ~PARENB;        // No parity
    options.c_cflag &= ~CSTOPB;        // 1 stop bit
    options.c_cflag &= ~CRTSCTS;       // No hardware flow control
    tcsetattr(uart_fd, TCSANOW, &options); // Apply the settings

    printf("UART Parent-Child Loopback Test using %s\n", UART_DEVICE);

    // Fork the process to create a parent and a child
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        close(uart_fd);
        return -1;
    }
    
    if (pid == 0) {
        // Child process will read from UART
        child_process(uart_fd);
    } else {
        // Parent process will write to UART based on user input
        parent_process(uart_fd);
    }

    // Close the UART device when done (this won't be reached unless the program is terminated)
    close(uart_fd);
    return 0;
}

