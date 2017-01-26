#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>

#define delayTime 1000



int init(char *gpioNum, char *direction);
void forward(FILE *a1, FILE *a2, FILE *b1, FILE *b2);
void backwards(FILE *a1, FILE *a2, FILE *b1, FILE *b2);
void right(FILE *c1, FILE *c2, FILE *d1, FILE *d2);
void left(FILE *c1, FILE *c2, FILE *d1, FILE *d2);
int UART_init(int *uartfs);

int main(void) 
{
	FILE *a1;
	FILE *a2;
	FILE *b1;
	FILE *b2;
	FILE *c1;
	FILE *c2;
	FILE *d1;
	FILE *d2;
	FILE *readForward;
	FILE *readBackward;
	FILE *readRight;
	FILE *readLeft;

	int result;
	int goForward;
	int goBackwards;
	int goRight;
	int goLeft;

	int uartfs = -1;
	unsigned char rx_buffer[256];
	int rx_length = 0;
	result = UART_init(&uartfs);	
	result = init("4", "out");				//initialize gpio4 for output
	if (result == 1) {
		perror("gpio 4 init");
		return 0;
	}
	result = init("17", "out");				//initialize gpio17 for output
	if (result == 1) {
		perror("gpio 17 init");
		return 0;
	}
	result = init("27", "out");				//initialize gpio27 for output
	if (result == 1) {
		perror("gpio 27 init");
		return 0;
	}
	result = init("22", "out");				//initialize gpio22 for output
	if (result == 1) {
		perror("gpio 22 init");
		return 0;
	}
	result = init("23","in");
	if (result == 1) {
		perror("gpio 23 init");
		return 0;
	}	
	result = init("24","in");
	if (result == 1) {
		perror("gpio 24 init");
		return 0;
	}
	result = init("6", "out");				//initialize gpio4 for output
	if (result == 1) {
		perror("gpio 6 init");
		return 0;
	}
	result = init("13", "out");				//initialize gpio17 for output
	if (result == 1) {
		perror("gpio 13 init");
		return 0;
	}
	result = init("19", "out");				//initialize gpio27 for output
	if (result == 1) {
		perror("gpio 19 init");
		return 0;
	}
	result = init("26", "out");				//initialize gpio22 for output
	if (result == 1) {
		perror("gpio 26 init");
		return 0;
	}
	result = init("16","in");
	if (result == 1) {
		perror("gpio 16 init");
		return 0;
	}	
	result = init("20","in");
	if (result == 1) {
		perror("gpio 20 init");
		return 0;
	}
	a1 = fopen("/sys/class/gpio/gpio4/value","w+");
	if (a1 == NULL) {
		perror("a1 fopen");
		return 0;
	}
	b1 = fopen("/sys/class/gpio/gpio17/value","w+");
	if (b1 == NULL) {
		perror("b1 fopen");
		return 0;
	}
	a2 = fopen("/sys/class/gpio/gpio27/value","w+");
	if (a2 == NULL) {
		perror("a2 fopen");
		return 0;
	}
	b2 = fopen("/sys/class/gpio/gpio22/value","w+");
	if (b2 == NULL) {
		perror("b2 fopen");
		return 0;
	}
	c1 = fopen("/sys/class/gpio/gpio6/value","w+");
	if (c1 == NULL) {
		perror("c1 fopen");
		return 0;
	}
	d1 = fopen("/sys/class/gpio/gpio13/value","w+");
	if (d1 == NULL) {
		perror("d1 fopen");
		return 0;
	}
	c2 = fopen("/sys/class/gpio/gpio19/value","w+");
	if (c2 == NULL) {
		perror("c2 fopen");
		return 0;
	}
	d2 = fopen("/sys/class/gpio/gpio26/value","w+");
	if (d2 == NULL) {
		perror("d2 fopen");
		return 0;
	}
	 
	while (1) {
		if (uartfs != -1) {
			rx_length = read(uartfs, (void*)rx_buffer, 255);
			if (rx_length < 0) {
				perror("uartfs read");
				return 1;
			} else if (rx_length == 0) {
				printf("No data\n");
			} else {
				rx_buffer[rx_length] = '\0';
				printf("%i bytes read : %s\n", rx_length, rx_buffer);
			}
		}
		if (goForward) {
			forward(a1, a2, b1, b2);
		} else if (goBackwards) {
			backwards(a1, a2, b1, b2);
		}
		if (goRight) {
			right(c1, c2, d1, d2);
		} else if (goLeft) {
			left (c1, c2, d1, d2);
		}
		if (!goForward && !goBackwards) {
			fprintf(a1, "0");
			fprintf(a2, "0");
			fprintf(b1, "0");
			fprintf(b2, "0");
		} 

		if (!goRight && !goLeft) {
			fprintf(c1, "0");
			fprintf(c2, "0");
			fprintf(d1, "0");
			fprintf(d2, "0");
		}
			
		fclose(readForward);
		fclose(readBackward);
		fclose(readRight);
		fclose(readLeft);
	}
	fclose(a1);
	fclose(a2);
	fclose(b1);
	fclose(b2);
	fclose(c1);
	fclose(c2);
	fclose(d1);
	fclose(d2);
	fclose(readForward);
	fclose(readBackward);
	fclose(readRight);
	fclose(readLeft);
	return 0;
}

int init(char *gpioNum, char *direction)
{
	FILE *fd_init;
	char temp[35] = "/sys/class/gpio/gpio";
	fd_init = fopen("/sys/class/gpio/export", "w+");		//opens file for GPIO enabling
	if (fd_init == NULL) {						//error checking for successful file open
		perror("fopen()");
		return 1;
	}
	fprintf(fd_init,"%s", gpioNum);					//enables GPIO gpioNum
	fclose(fd_init);
	strcat(temp, gpioNum);
	strcat(temp, "/direction");
	fd_init = fopen(temp, "w+");
	if (fd_init == NULL) {
		perror("fopen2()");
		return 1;
	} 
	fprintf(fd_init, "%s", direction);
	fclose(fd_init);
	return 0;
}

void forward(FILE *a1, FILE *a2, FILE *b1, FILE *b2)
{

		rewind(a2);
		fprintf(a2, "1");
		usleep(delayTime);
		rewind(a1);
		fprintf(a1, "0");
		usleep(delayTime);
		rewind(b1);
		fprintf(b1, "1");
		usleep(delayTime);
		rewind(a2);
		fprintf(a2, "0");
		usleep(delayTime);
		rewind(b2);
		fprintf(b2, "1");
		usleep(delayTime);
		rewind(b1);
		fprintf(b1, "0");
		usleep(delayTime);
		rewind(a1);
		fprintf(a1, "1");
		usleep(delayTime);
		rewind(b2);
		fprintf(b2, "0");
		usleep(delayTime);
}
void backwards(FILE *a1, FILE *a2, FILE *b1, FILE *b2)
{

		rewind(b2);
		fprintf(b2, "1");
		usleep(delayTime);
		rewind(a1);
		fprintf(a1, "0");
		usleep(delayTime);
		rewind(b1);
		fprintf(b1, "1");
		usleep(delayTime);
		rewind(b2);
		fprintf(b2, "0");
		usleep(delayTime);
		rewind(a2);
		fprintf(a2, "1");
		usleep(delayTime);
		rewind(b1);
		fprintf(b1, "0");
		usleep(delayTime);
		rewind(a1);
		fprintf(a1, "1");
		usleep(delayTime);
		rewind(a2);
		fprintf(a2, "0");
		usleep(delayTime);
}
void right(FILE *c1, FILE *c2, FILE *d1, FILE *d2)
{

		rewind(c2);
		fprintf(c2, "1");
		usleep(delayTime);
		rewind(c1);
		fprintf(c1, "0");
		usleep(delayTime);
		rewind(d1);
		fprintf(d1, "1");
		usleep(delayTime);
		rewind(c2);
		fprintf(c2, "0");
		usleep(delayTime);
		rewind(d2);
		fprintf(d2, "1");
		usleep(delayTime);
		rewind(d1);
		fprintf(d1, "0");
		usleep(delayTime);
		rewind(c1);
		fprintf(c1, "1");
		usleep(delayTime);
		rewind(d2);
		fprintf(d2, "0");
		usleep(delayTime);
}
void left(FILE *c1, FILE *c2, FILE *d1, FILE *d2)
{

		rewind(d2);
		fprintf(d2, "1");
		usleep(delayTime);
		rewind(c1);
		fprintf(c1, "0");
		usleep(delayTime);
		rewind(d1);
		fprintf(d1, "1");
		usleep(delayTime);
		rewind(d2);
		fprintf(d2, "0");
		usleep(delayTime);
		rewind(c2);
		fprintf(c2, "1");
		usleep(delayTime);
		rewind(d1);
		fprintf(d1, "0");
		usleep(delayTime);
		rewind(c1);
		fprintf(c1, "1");
		usleep(delayTime);
		rewind(c2);
		fprintf(c2, "0");
		usleep(delayTime);
}

int UART_init(int *uartfs) 
{	
	*uartfs = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);		//open in non-blocking read/write mode
	if (*uartfs == -1) {
		perror("UART open");
		return 1;
	}
	struct termios options;
	tcgetattr(*uartfs, &options);
	options.c_cflag = B4800 | CS8 | CLOCAL | CREAD;				//sets baud rate to 4800 bps
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(*uartfs, TCIFLUSH);
	tcsetattr(*uartfs, TCSANOW, &options);
	return 0;
}
