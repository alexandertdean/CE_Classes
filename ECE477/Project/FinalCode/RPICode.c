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

int gpio_init(char *gpioNum, char *direction);


int main () 
{
	time_t rawtime;
	struct tm * timeinfo;
	int spi_fd;
	FILE *speaker_fd;
	FILE *greenled_fd;
	FILE *yellowled_fd;
	FILE *redled_fd;
	FILE *state_fd;
	FILE *button_fd;
	FILE *log_fd;
	int bufVal;
	char state;
	char prevState; 
	int pressed;
	unsigned char data[3];
	int result;
	int buzz;
	int test;

	gpio_init("4", "out");
	gpio_init("17", "out");
	gpio_init("27", "out");
	gpio_init("22", "out");
	gpio_init("23", "in");

	speaker_fd = fopen("/sys/class/gpio/gpio4/value","w+");					//opens GPIO value file for driving speaker
	if (speaker_fd == NULL) {
		perror("FOPEN GPIO 4 VALUE");
		return 1;
	}
	redled_fd = fopen("/sys/class/gpio/gpio17/value", "w+");				//opens GPIO value file for driving the red LED
	if (redled_fd == NULL) {
		perror("FOPEN GPIO 17 VALUE");
		return 1;
	}
	yellowled_fd = fopen("/sys/class/gpio/gpio27/value", "w+");				//opens GPIO value file for driving the yellow LED
	if (yellowled_fd == NULL) {
		perror("FOPEN GPIO 27 VALUE");
		return 1;
	}
	greenled_fd = fopen("/sys/class/gpio/gpio22/value", "w+");				//opens GPIO value file for driving the green LED
	if (greenled_fd == NULL) {
		perror("FOPEN GPIO 22 VALUE");
		return 1;
	}
	//Open SPI device
	spi_fd = open("/dev/spidev0.0", O_RDWR);								//opens SPI device
	if (spi_fd < 0) {
		perror("Open SPI");
		return 1;
	}
	//Set SPI MODE_0
	bufVal =  SPI_MODE_0;
	result = ioctl(spi_fd, SPI_IOC_WR_MODE, &bufVal);						//set to mode zero, which is standard. SPI read/write happens on rising edge
	if (result != 0) {
		perror("ioctl1");
		return 1;
	}

	struct spi_ioc_transfer spi = {
		.tx_buf = (unsigned long) &data,									//transmit and receive buffers set to data[]
		.rx_buf = (unsigned long) &data,
		.len = 3,															//length of read is 3 bytes
		.delay_usecs = 0,
		.speed_hz = 10000,													//sets speed of transfer
		.bits_per_word = 8,													//8 bits per byte of data
		.cs_change = 0,
	};
	log_fd = fopen("log.txt", "w+");										//opens log file
	while(1) {
		state_fd = fopen("alarm_state.txt", "r+");
		if (state_fd == NULL) {
			perror("FUCK");	
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
		result = (data[1] << 8) | data[2];									//shifts one byte and combines it with another to make 16 bit data
		if (result > 100 && state == 'A') {									//checks if alarm is tripped
			rewind(state_fd);
			fprintf(state_fd, "T");
		}
// CHECK STATE OF ALARM AND TURN ON LEDS AND SPEAKER ACCORDINGLY	
		rewind(state_fd);
		fscanf(state_fd, "%c", &state);
		if (state != prevState) {											//checks for state change
			time(&rawtime);
			timeinfo = localtime(&rawtime);																																						//gets current time
			fprintf(log_fd, "%02d-%02d-%02d,%02d:%02d:%02d,", timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);		//prints time and data into log
			fflush(log_fd);
			switch(state) {												//pritns new state of alarm in alarm info
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
		switch (state) {													//handles states
		case 'T': 
			prevState = 'T';
			fprintf(redled_fd, "1");										//Red LED turned on
			fprintf(yellowled_fd, "0");
			fprintf(greenled_fd, "0");
			rewind(redled_fd);
			rewind(yellowled_fd);
			rewind(greenled_fd);
			for (buzz = 0; buzz < 250; buzz++) {							//buzz buzzer for a short bit
				fprintf(speaker_fd, "1");
				usleep(1000);
				rewind(speaker_fd);
				fprintf(speaker_fd, "0");
				usleep(1000);
				rewind(speaker_fd);
			}
			usleep(200000);													//silence for a short bit
			break;
		case 'A':
			prevState = 'A';
			fprintf(greenled_fd, "0");
			fprintf(yellowled_fd, "1");										//turns on yellow LED
			fprintf(redled_fd, "0");
			rewind(greenled_fd);
			rewind(yellowled_fd);
			rewind(redled_fd);
			break;
		case 'D':
			prevState = 'D';
			fprintf(greenled_fd, "1");										//turns on green LED
			fprintf(yellowled_fd, "0");
			fprintf(redled_fd, "0");
			rewind(greenled_fd);
			rewind(yellowled_fd);
			rewind(redled_fd);
			break;
		default:
			printf("Invalid state read from file\n");						//THIS SHOULD NEVER HAPPEN
			break;
		}
		button_fd = fopen("/sys/class/gpio/gpio23/value", "r+");			//opens push button value file
		fscanf(button_fd, "%d", &pressed);
		fclose(button_fd);
		if (pressed) {
			switch (state) {												//handles push button presses based on state of alarm
			case 'T' :
				for (test = 0; test < 5; test++) {							//continously checks if push button is pressed for five-ish seconds
					button_fd = fopen("/sys/class/gpio/gpio23/value", "r+");
					fscanf(button_fd, "%d", &pressed);
					fclose(button_fd);
					if (!pressed) {
						break;
					}
			 		for (buzz = 0; buzz < 250; buzz++) {					//continues to buzz buzzer while in for loop
						fprintf(speaker_fd, "1");
						usleep(1000);
						rewind(speaker_fd);
						fprintf(speaker_fd, "0");
						usleep(1000);
						rewind(speaker_fd);
					}
					usleep(200000);
				}
					if (pressed) {											//changes state
						rewind(state_fd);
						fprintf(state_fd, "D");
						fprintf(redled_fd, "0");
						fprintf(greenled_fd, "1");
						rewind(redled_fd);
						rewind(greenled_fd);
						usleep(2000000);
					}
					break;
			case 'D' :														//changes state to armed after five seconds
				usleep(5000000);
				rewind(state_fd);
				fprintf(state_fd, "A");
				fprintf(yellowled_fd, "1");
				fprintf(greenled_fd, "0");
				rewind(yellowled_fd);
				rewind(greenled_fd);
				break;
			case 'A' :														//disarms
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
