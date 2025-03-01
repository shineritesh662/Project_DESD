#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

#define GPS_DEVICE "/dev/ttyO4" // UART4 for BeagleBone (P9_11 and P9_13)

int configure_serial_port(int fd) {
    struct termios options;

    // Get current serial port options
    if (tcgetattr(fd, &options) < 0) {
        perror("tcgetattr failed");
        return -1;
    }

    // Set the baud rate to 9600
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    // Set 8N1 (8 data bits, no parity, 1 stop bit)
    options.c_cflag &= ~PARENB; // No parity
    options.c_cflag &= ~CSTOPB; // 1 stop bit
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8; // 8 data bits

    // Disable hardware flow control
    options.c_cflag &= ~CRTSCTS;

    // Set raw input/output mode
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // Disable software flow control
    options.c_iflag &= ~(IXON | IXOFF | IXANY);

    // Apply the settings
    if (tcsetattr(fd, TCSANOW, &options) < 0) {
        perror("tcsetattr failed");
        return -1;
    }

    return 0;
}

void read_gps_data(int fd) {
    char buffer[256];
    int bytes_read;

    while (1) {
        bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read < 0) {
            perror("Read error");
            break;
        } else if (bytes_read > 0) {
            buffer[bytes_read] = '\0';  // Null-terminate the string
            printf("%s", buffer);  // Print the NMEA data to the console
        }
    }
}

int main() {
    int fd;

    // Open the serial port
    fd = open(GPS_DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0) {
        perror("Unable to open GPS device");
        return 1;
    }

    // Configure the serial port
    if (configure_serial_port(fd) < 0) {
        close(fd);
        return 1;
    }

    printf("Reading GPS data...\n");

    // Read data from the GPS
    read_gps_data(fd);

    // Close the serial port
    close(fd);

    return 0;
}

