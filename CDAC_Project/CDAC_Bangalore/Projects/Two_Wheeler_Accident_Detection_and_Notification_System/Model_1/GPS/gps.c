#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define GPS_DEVICE "/dev/ttyS1" // Change to your GPS UART port

// Function to configure the UART port
int configure_uart(int fd) {
    struct termios options;

    // Get the current options for the port
    tcgetattr(fd, &options);

    // Set the baud rate to 9600
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    // 8N1: 8 data bits, no parity, 1 stop bit
    options.c_cflag &= ~PARENB; // No parity
    options.c_cflag &= ~CSTOPB; // 1 stop bit
    options.c_cflag &= ~CSIZE;  // Clear data size
    options.c_cflag |= CS8;     // 8 data bits

    // Enable the receiver and set local mode
    options.c_cflag |= (CLOCAL | CREAD);

    // Set raw input mode (no processing)
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY); // No software flow control
    options.c_oflag &= ~OPOST;                 // Raw output

    // Apply the configuration
    tcsetattr(fd, TCSANOW, &options);

    return 0;
}

// Function to extract latitude and longitude from an NMEA GPGGA sentence
void parse_gpgga(const char *nmea_sentence) {
    char time[10], lat[15], lon[15], ns, ew, alt[10];
    char *token = strtok((char *)nmea_sentence, ",");

    // Extract individual fields
    int field = 0;
    while (token != NULL) {
        field++;
        if (field == 2) strcpy(time, token);         // UTC Time
        if (field == 3) strcpy(lat, token);          // Latitude
        if (field == 4) ns = token[0];              // N/S Indicator
        if (field == 5) strcpy(lon, token);         // Longitude
        if (field == 6) ew = token[0];              // E/W Indicator
        if (field == 10) strcpy(alt, token);        // Altitude

        token = strtok(NULL, ",");
    }

    // Print extracted data
    printf("Time (UTC): %s\n", time);
    printf("Latitude: %s %c\n", lat, ns);
    printf("Longitude: %s %c\n", lon, ew);
    printf("Altitude: %s meters\n", alt);
}

int main() {
    int fd;
    char buffer[256];
    ssize_t bytes_read;

    // Open the GPS device
    fd = open(GPS_DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        perror("Failed to open the GPS device");
        return -1;
    }

    // Configure the UART port
    configure_uart(fd);

    printf("Reading GPS data...\n");

    while (1) {
        // Read data from the GPS module
        bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // Null-terminate the string

            // Check for GPGGA sentence and parse it
            if (strstr(buffer, "$GPGGA") != NULL) {
                parse_gpgga(buffer);
            }
        }

        usleep(500000); // Sleep for 500ms
    }

    // Close the GPS device
    close(fd);

    return 0;
}
