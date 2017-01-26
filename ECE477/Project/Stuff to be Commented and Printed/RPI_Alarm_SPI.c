#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <time.h>

// Initialize GPIO PORTS
int gpio_init(char *gpioNum, char *direction);
// Open files specified
int open_file(FILE **speaker_fd, FILE **redled_fd, FILE **yellowled_fd, FILE **greenled_fd);
// Setup the Serial Peripheral Interface
int setup_spi();
// Run alarm in while(1) to continuously run
int run_alarm(FILE * state_fd, int spi_fd, int result, unsigned char data[], struct spi_ioc_transfer spi, FILE *speaker_fd, FILE *redled_fd, FILE *yellowled_fd, FILE *greenled_fd);

int main(void)
{
	// Initialize Variables
	int spi_fd, result, bufVal, fd;
	FILE *speaker_fd, *greenled_fd, *yellowled_fd, *redled_fd, *state_fd, *button_fd;
	unsigned char data[3];
	// Initialize GPIO Pins
	gpio_init("4", "out");
	gpio_init("17", "out");
	gpio_init("27", "out");
	gpio_init("22", "out");
	gpio_init("23", "in");
	// Open file pointers
	fd = open_file(&speaker_fd, &redled_fd, &yellowled_fd, &greenled_fd);
	if (fd == 0)
		perror("File Pointer Open Error");
	// Open serial port
	spi_fd = setup_spi();
	//Set SPI MODE_0
	bufVal =  SPI_MODE_0;
	result = ioctl(spi_fd, SPI_IOC_WR_MODE, &bufVal);			//set to mode zero, which is standard
	if (result != 0) {
		perror("ioctl1");
		return 1;
	}
	// Set up SPI Data Stucture
	struct spi_ioc_transfer spi = {
		.tx_buf = (unsigned long) &data,
		.rx_buf = (unsigned long) &data,
		.len = 3,
		.delay_usecs = 0,
		.speed_hz = 10000,
		.bits_per_word = 8,
		.cs_change = 0,
	};
	// Run alarm and continuously check SPI to decide what to output to text files
	run_alarm(state_fd, spi_fd, result, data, spi, speaker_fd, redled_fd, yellowled_fd, greenled_fd);
	// Exit if this point is reached
	return 0;
}

int gpio_init(char *gpioNum, char *direction)
{
	FILE *fd;
	char temp[35] = "/sys/class/gpio/gpio";
	fd = fopen("/sys/class/gpio/export", "w+");
	if (fd == NULL) {
		perror("FOPEN EXPORT");
		return 1;
	}
	fprintf(fd, "%s", gpioNum);
	fclose(fd);
	strcat(temp, gpioNum);
	strcat(temp, "/direction");
	fd = fopen(temp, "w+");
	if (fd == NULL) {
		perror("FOPEN DIRECTION");
		return 1;
	}
	fprintf(fd, "%s", direction);
	fclose(fd);
	return 0;
}

int open_file(FILE **speaker_fd, FILE **redled_fd, FILE **yellowled_fd, FILE **greenled_fd)
{
	*speaker_fd = fopen("/sys/class/gpio/gpio4/value","w+");
	if (*speaker_fd == NULL) {
		perror("FOPEN GPIO 4 VALUE");
		return 1;
	}
	*redled_fd = fopen("/sys/class/gpio/gpio17/value", "w+");
	if (*redled_fd == NULL) {
		perror("FOPEN GPIO 17 VALUE");
		return 1;
	}
	*yellowled_fd = fopen("/sys/class/gpio/gpio27/value", "w+");
	if (*yellowled_fd == NULL) {
		perror("FOPEN GPIO 27 VALUE");
		return 1;
	}
	*greenled_fd = fopen("/sys/class/gpio/gpio22/value", "w+");
	if (*greenled_fd == NULL) {
		perror("FOPEN GPIO 22 VALUE");
		return 1;
	}
	return 0;
}

int setup_spi()
{
	int spi_fd;
	//Open SPI device
	spi_fd = open("/dev/spidev0.0", O_RDWR);
	if (spi_fd < 0) {
		perror("Open SPI");
		return 1;
	}
	return spi_fd;
}

int run_alarm(FILE * state_fd, int spi_fd, int result, unsigned char data[], struct spi_ioc_transfer spi, FILE *speaker_fd, FILE *redled_fd, FILE *yellowled_fd, FILE *greenled_fd)
{
	time_t rawtime;
	struct tm * timeinfo;
	int pressed, buzz, test;
	char state, prevState;
	FILE *button_fd,*	log_fd;
	log_fd = fopen("log.txt", "w+");
	while(1){
		state_fd = fopen("alarm_state.txt", "r+");
		if (state_fd == NULL) {
			perror("ALARM STATE FILE OPEN");
			return 1;
		}
		//DO SPI STUFF
		data[0] = 0x01;								//start bit
		data[1] = 0x10;								//sets CH2 as desired read
		data[2] = 0x00;								//padding for output
		result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);
		if (result < 0) {
			perror("ioctl2");
			return 1;
		}
		result = (data[1] << 8) | data[2];
		if (result > 100 && state == 'A') {
			rewind(state_fd);
			fprintf(state_fd, "T");
		}
// CHECK STATE OF ALARM AND TURN ON LEDS AND SPEAKER ACCORDINGLY
		rewind(state_fd);
		fscanf(state_fd, "%c", &state);
		if (state != prevState) {
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			fprintf(log_fd, "%02d-%02d-%02d,%02d:%02d:%02d,", timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
			fflush(log_fd);
			switch(state) {
			case 'T':
				fprintf(log_fd, "Alarm Triggered\n");
				break;
			case 'A':
				fprintf(log_fd, "Alarm Armed\n");
				break;
			case 'D':
				fprintf(log_fd, "Alarm Disarmed\n");
				break;
			default:
				fprintf(log_fd, "Something is wrong\n");
			}
			fflush(log_fd);
		}
		switch (state) {
		case 'T':
			prevState = 'T';
			fprintf(redled_fd, "1");
			fprintf(yellowled_fd, "0");
			fprintf(greenled_fd, "0");
			rewind(redled_fd);
			rewind(yellowled_fd);
			rewind(greenled_fd);
			for (buzz = 0; buzz < 250; buzz++) {
				fprintf(speaker_fd, "1");
				usleep(1000);
				rewind(speaker_fd);
				fprintf(speaker_fd, "0");
				usleep(1000);
				rewind(speaker_fd);
			}
			usleep(200000);
			break;
		case 'A':
			prevState = 'A';
			fprintf(greenled_fd, "0");
			fprintf(yellowled_fd, "1");
			fprintf(redled_fd, "0");
			rewind(greenled_fd);
			rewind(yellowled_fd);
			rewind(redled_fd);
			break;
		case 'D':
			prevState = 'D';
			fprintf(greenled_fd, "1");
			fprintf(yellowled_fd, "0");
			fprintf(redled_fd, "0");
			rewind(greenled_fd);
			rewind(yellowled_fd);
			rewind(redled_fd);
			break;
		default:
			printf("Invalid state read from file\n");
			break;
		}
		button_fd = fopen("/sys/class/gpio/gpio23/value", "r+");
		fscanf(button_fd, "%d", &pressed);
		fclose(button_fd);
		if (pressed) {
			switch (state) {
			case 'T' :
				for (test = 0; test < 5; test++) {
					button_fd = fopen("/sys/class/gpio/gpio23/value", "r+");
					fscanf(button_fd, "%d", &pressed);
					fclose(button_fd);
					if (!pressed) {
						break;
					}
			 		for (buzz = 0; buzz < 250; buzz++) {
						fprintf(speaker_fd, "1");
						usleep(1000);
						rewind(speaker_fd);
						fprintf(speaker_fd, "0");
						usleep(1000);
						rewind(speaker_fd);
					}
					usleep(200000);
				}
					if (pressed) {
						rewind(state_fd);
						fprintf(state_fd, "D");
						fprintf(redled_fd, "0");
						fprintf(greenled_fd, "1");
						rewind(redled_fd);
						rewind(greenled_fd);
						usleep(2000000);
					}
					break;
			case 'D' :
				usleep(5000000);
				rewind(state_fd);
				fprintf(state_fd, "A");
				fprintf(yellowled_fd, "1");
				fprintf(greenled_fd, "0");
				rewind(yellowled_fd);
				rewind(greenled_fd);
				break;
			case 'A' :
				rewind(state_fd);
				fprintf(state_fd, "D");
				fprintf(yellowled_fd, "0");
				fprintf(greenled_fd, "1");
				rewind(greenled_fd);
				rewind(yellowled_fd);
				usleep(2000000);
				break;
			default :
				printf("Invalid state read from file\n");
			}
		}
	fclose(state_fd);
	}
	fclose(log_fd);
	fclose(greenled_fd);
	fclose(yellowled_fd);
	fclose(redled_fd);
	fclose(speaker_fd);
	close(spi_fd);
}
