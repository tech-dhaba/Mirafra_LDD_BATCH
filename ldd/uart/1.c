#include <stdio.h>
#include <fcntl.h>      // File control definitions
#include <termios.h>    // POSIX terminal control definitions
#include <unistd.h>     // UNIX standard function definitions
#include <string.h>     // String functions

int main() {
    int uart_fd;
    char buffer[256];
    int bytes_read;

    // Open the UART device
    uart_fd = open("/dev/serial1", O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart_fd == -1) {
        perror("Failed to open UART");
        return -1;
    }

    // Configure the UART
    struct termios options;
    tcgetattr(uart_fd, &options);  // Get current settings
    cfsetispeed(&options, B9600);  // Set input baud rate to 9600
    cfsetospeed(&options, B9600);  // Set output baud rate to 9600
    options.c_cflag = CS8 | CLOCAL | CREAD;  // 8 data bits, no parity, 1 stop bit
    options.c_iflag = IGNPAR;      // Ignore framing and parity errors
    options.c_oflag = 0;
    options.c_lflag = 0;           // Set non-canonical mode

    tcflush(uart_fd, TCIFLUSH);    // Flush the input buffer
    tcsetattr(uart_fd, TCSANOW, &options);  // Apply the settings

    // Read from the UART buffer
    while (1) {
        memset(buffer, 0, sizeof(buffer));  // Clear the buffer
        bytes_read = read(uart_fd, buffer, sizeof(buffer) - 1);  // Read from UART
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';  // Null-terminate the string
            printf("Data received: %s\n", buffer);
        }
    }

    // Close the UART device
    close(uart_fd);

    return 0;
}

