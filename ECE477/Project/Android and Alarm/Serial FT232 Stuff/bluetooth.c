#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
int main(int argc, char* argv)
{
	struct termios tio;
	int tty_fd;
	FILE *fp;
	int i = 0;
	char phone[3];
	tty_fd = open("/dev/ttyUSB0", O_RDWR | O_NONBLOCK);
	cfsetospeed(&tio, B9600);            // set baud to 9600
	cfsetispeed(&tio, B9600);            // set baud to 9600
	tcsetattr(tty_fd, TCSANOW, &tio);
	// Clear Junk from State Buffer 
	for (i = 0; i < 4; ++i)
		phone[i] = ' ';
	// Continuously scan for a state change from bluetooth
	while (1)
	{
		// Read from RX to see what HC-05 TX sent
		if (read(tty_fd, &phone, 3)){
			// Check to see what state to change to
			if (phone[0] == 'D'){
				fp = fopen("colors", "w+");
				fprintf(fp, "D");
				fclose(fp);
			}
			else if (phone[0] == 'A'){
				fp = fopen("colors", "w+");
				fprintf(fp, "A");
				fclose(fp);
			}
			else if (phone[0] == 'T'){
				fp = fopen("colors", "w+");
				fprintf(fp, "T");
				fclose(fp);
			}
			// Just to show in stdout
			printf("%s\n", phone);
			// Clear buffer so doesnt keep overwritting
			for (i = 0; i < 4; ++i)
				phone[i] = ' ';
		}
	}
	close(tty_fd); // Close serial
	return 0;
}


