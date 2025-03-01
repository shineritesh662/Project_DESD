#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <linux/i2c-dev.h>
#include <string.h>

#define ADXL345_SLAVE_ADDR 0x53    		// Slave address of I2C - ADXL345
#define I2C_FILE_PATH "/dev/i2c-2" 		// I2C device file directory

// The Register addresses are mapped by referring the ADXL345 Datasheet provided by ANALOG DEVICES

#define ADXL345_BW_RATE_REG	0x2C     	// Bandwidth rate control register
#define ADXL345_POWERCTL_REG 	0x2D    	// Power control register
#define ADXL345_DATA_FORMAT_REG 0x31 		// Data format register
#define ADXL345_REG_DATAX0 	0x32      	// Start of data registers
#define ADXL345_REG_DATAX1 	0x33
#define ADXL345_REG_DATAY0 	0x34
#define ADXL345_REG_DATAY1 	0x35
#define ADXL345_REG_DATAZ0 	0x36
#define ADXL345_REG_DATAZ1 	0x37

// Function to initialize the ADXL345

int adxl345_init()
{
    char I2C_Communicate[2]; 	// Two-element buffer to hold register address and data
    int file;            	// Local file descriptor or file pointer (fp)

    // Open the I2C file
    
    if ((file = open(I2C_FILE_PATH, O_RDWR)) < 0)
    {
        perror("Failed to open the I2C bus\n");
        return -1;
    }

    // Connect to the ADXL345 slave device
    
    if (ioctl(file, I2C_SLAVE, ADXL345_SLAVE_ADDR) < 0)		// To configure the file descriptor to communicate with ADXL345(0x53) I2C slave device  
    {
        perror("Failed to connect to the sensor\n");
        close(file);
        return -1;
    }

    // Configure the BW_RATE register (3200 Hz, normal mode)
    
    I2C_Communicate[0] = ADXL345_BW_RATE_REG;
    I2C_Communicate[1] = 0x0F;
    if (write(file, I2C_Communicate, 2) != 2)
    {
        perror("Failed to write to the BW_RATE register\n");
        close(file);
        return -1;
    }

    // Configure the POWER_CTL register to enable measurement mode as the ADXL345 powers up in standby mode with minimum power consumption.

    I2C_Communicate[0] = ADXL345_POWERCTL_REG;
    I2C_Communicate[1] = 0x08;
    if (write(file, I2C_Communicate, 2) != 2)
    {
        perror("Failed to write to the POWER_CTL register\n");
        close(file);
        return -1;
    }

    // Configure the DATA_FORMAT register (full resolution, ±2g range)
    
    I2C_Communicate[0] = ADXL345_DATA_FORMAT_REG;
    I2C_Communicate[1] = 0x08;
    if (write(file, I2C_Communicate, 2) != 2)
    {
        perror("Failed to write to the DATA_FORMAT register\n");
        close(file);
        return -1;
    }

    printf("ADXL345 initialized successfully\n");

    // Close the I2C file descriptor
    
    close(file);

    return 0;
}

// Function to read accelerometer data

int adxl345_read(short *acc_x, short *acc_y, short *acc_z)
{
    int file;                                   	// File descriptor for I2C (fp)
    char value[6];                             		// Buffer to hold X, Y, Z axis data (6 bytes)
    char reg_pointer[1] = {ADXL345_REG_DATAX0}; 	// Start data register address

    // Open the I2C file
    
    if ((file = open(I2C_FILE_PATH, O_RDWR)) < 0)
    {
        perror("Failed to open the I2C bus");
        return -1;
    }

    // Connect to the ADXL345 slave device
    
    if (ioctl(file, I2C_SLAVE, ADXL345_SLAVE_ADDR) < 0)
    {
        perror("Failed to connect to the sensor");
        close(file);
        return -1;
    }

    // Set the register pointer to the start of the data registers
    
    if (write(file, reg_pointer, 1) != 1)
    {
        perror("Failed to set the register pointer");
        close(file);
        return -1;
    }

    // Read 6 bytes of accelerometer data (X0, X1, Y0, Y1, Z0, Z1) starting from register address 0x32 to 0x37
    
    if (read(file, value, 6) != 6)
    {
        perror("Failed to read accelerometer data");
        close(file);
        return -1;
    }

    // Combine high and low bytes to get 16-bit values for each axis
    
    *acc_x = (value[1] << 8) | value[0]; // X-axis
    *acc_y = (value[3] << 8) | value[2]; // Y-axis
    *acc_z = (value[5] << 8) | value[4]; // Z-axis

    // Close the I2C file descriptor
    
    close(file);

    return 0;
}

int main()
{

     // Use UART1 for board to board communication during accident detection

    int uart1_file = -1;						// uart file descriptor
    uart1_file = open("/dev/ttyS1", O_RDWR | O_NOCTTY | O_NDELAY); 	// Open the UART1 device file

    if (uart1_file == -1) 
    {
        printf("Error - Unable to open UART. Ensure it is not in use by another application\n");
        return -1;
    }

    // Configure the UART
    
    struct termios uart_frame;
    tcgetattr(uart1_file, &uart_frame);
    uart_frame.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    uart_frame.c_iflag = IGNPAR;
    uart_frame.c_oflag = 0;
    uart_frame.c_lflag = 0;
    tcflush(uart1_file, TCIFLUSH);
    tcsetattr(uart1_file, TCSANOW, &uart_frame);

    // ADXL345 sensor initialization	

    short acc_x, acc_y, acc_z;

    // Initialize the ADXL345
    
    if (adxl345_init() != 0)
    {
        printf("Failed to initialize the ADXL345\n");
        return -1;
    }

    // Read accelerometer data and keep checking for any accident detection
    
    while (1)
    {
        if (adxl345_read(&acc_x, &acc_y, &acc_z) != 0)
        {
            printf("Failed to read accelerometer data\n");
            return -1;
        }

        // Print the accelerometer data
        
        printf("Accelerometer Data:\n");
        printf("X: %d\n", acc_x);
        printf("Y: %d\n", acc_y);
        printf("Z: %d\n", acc_z);

	if(acc_z < 170)
	{
		unsigned char tx_buffer[20];
    		unsigned char *p_tx_buffer;
    		p_tx_buffer = &tx_buffer[0];
    		strcpy(p_tx_buffer, "Accident Detected");
    		int count = write(uart1_file, &tx_buffer[0], strlen(p_tx_buffer));

    		if (count < 0) 
    		{
       		 printf("UART TX error\n");
    		}
    		
    		printf("Accident Detected\n");

    		// Close the UART
    
    		close(uart1_file);
    		return 0;
	}
	
        sleep(1);
    }

    return 0;
}
