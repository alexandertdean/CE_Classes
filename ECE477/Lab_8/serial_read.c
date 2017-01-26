#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

// Setup the serial port being written to
int setup_serial(int tty_fd);
// Check RX on FT232 to see if AVR sent temperature
void check(int tty_fd);

int main(void)
{
	// Initialize Variable
	int tty_fd;

	// Setup Serial Port of FT232
	tty_fd = setup_serial(tty_fd);

	// Check RX of FT232 to see if AVR sent temperature
	check(tty_fd);

	// Close serial
	close(tty_fd);

	return 0;      // Exit Program
}

int setup_serial(int tty_fd)
{
	// Initialize Variables
	struct termios tio;
	// Open Serial Port
	tty_fd = open("/dev/ttyUSB1", O_RDWR | O_NONBLOCK);
	cfsetospeed(&tio, B9600);           // Set baud to 9600
	cfsetispeed(&tio, B9600);           // Set baud to 9600
	tcsetattr(tty_fd, TCSANOW, &tio);   // Set serial attributes
	return tty_fd;                      // return serial file descriptor
}

void check(int tty_fd)
{
	// Initialize Variables
	char temperature[255];
	FILE *fp;
	int i = 0;
	memset(temperature, 0 ,sizeof(temperature)); // Clear junk initially in buffer
	// Continuously Loop to check if AVR sent temperature
	while(1){
		// Read from RX to see what AVR TX sent
		if (read(tty_fd, &temperature, 255)){
      printf("%s\n", temperature);
      sleep(10);  // Delay for 10 seconds so user can see temperature output (Better way would be FILE output with fprintf)
			memset(temperature, 0 ,sizeof(temperature)); // Clear buffer so doesnt keep overwritting
		}
	}
}
