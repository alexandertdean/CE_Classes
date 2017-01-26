#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
// Setup the serial port being written to
int setup_serial(int tty_fd);
// Check RX on FT232 to see if bluetooth sent information
void check(int tty_fd);

int main(void)
{
	// Initialize Variable
	int tty_fd;

	// Setup Serial Port of FT232
	tty_fd = setup_serial(tty_fd);

	// Check RX of FT232 to see if bluetooth sent information
	check(tty_fd);

	// Close serial
	close(tty_fd);

	return 0;      // Exit Program
}