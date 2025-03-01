#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

#define BAUDRATE B9600
#define PORT "/dev/ttyO1" // UART device on BeagleBone Black
#define BUFFER_SIZE 256

int uart_open(const char *port, int baudrate) {
    int uart_fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart_fd == -1) {
        perror("Unable to open UART");
        return -1;
    }

    struct termios options;
    tcgetattr(uart_fd, &options);
    cfsetispeed(&options, baudrate);
    cfsetospeed(&options, baudrate);
    options.c_cflag = CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart_fd, TCIFLUSH);
    tcsetattr(uart_fd, TCSANOW, &options);
    return uart_fd;
}

int main() {
    int uart_fd = uart_open(PORT, BAUDRATE);
    if (uart_fd == -1) {
        return -1;
    }

    char buffer[BUFFER_SIZE];
    while (1) {
        int bytes_read = read(uart_fd, buffer, BUFFER_SIZE - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Received: %s\n", buffer);
        }
        sleep(1);
    }

    close(uart_fd);
    return 0;
}

