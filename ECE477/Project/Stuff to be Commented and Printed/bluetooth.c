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
}int setup_serial(int tty_fd){	// Initialize Variables	struct termios tio;	// Open Serial Port	tty_fd = open("/dev/ttyUSB1", O_RDWR | O_NONBLOCK);	cfsetospeed(&tio, B9600);           // set baud to 9600	cfsetispeed(&tio, B9600);           // set baud to 9600	tcsetattr(tty_fd, TCSANOW, &tio);   // Set serial attributes	return tty_fd;}void check(int tty_fd){	// Initialize Variables	char phone[3];	FILE *fp;	int i = 0;	memset(phone, 0 ,sizeof(phone)); // Clear junk initially in buffer	// Continuously Loop to check if bluetooth sent information	while(1){		// Read from RX to see what HC-05 TX sent		if (read(tty_fd, &phone, 3)){			// Check to see what state to change to			if (phone[0] == 'D'){				fp = fopen("alarm_state.txt", "w+");				fprintf(fp, "D");				fclose(fp);			}			else if (phone[0] == 'A'){				fp = fopen("alarm_state.txt", "w+");				fprintf(fp, "A");				fclose(fp);			}			else if (phone[0] == 'T'){				fp = fopen("alarm_state.txt", "w+");				fprintf(fp, "T");				fclose(fp);			}			memset(phone, 0 ,sizeof(phone)); // Clear buffer so doesnt keep overwritting		}	}}