#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>


int main () 
{
	int spi_fd, i, j, bufVal;
	double value;
	unsigned char data[3];
	int result;

	//Open SPI device
	spi_fd = open("/dev/spidev0.0", O_RDWR);
	if (spi_fd < 0) {
		perror("Open");
		return 1;
	}
	//Set SPI MODE_0
	bufVal =  SPI_MODE_0;
	result = ioctl(spi_fd, SPI_IOC_WR_MODE, &bufVal);			//set to mode zero, which is standard 
	if (result != 0) {
		perror("ioctl1");
		return 1;
	}

	struct spi_ioc_transfer spi = {
		.tx_buf = (unsigned long) &data,
		.rx_buf = (unsigned long) &data,
		.len = 3,
		.delay_usecs = 0,
		.speed_hz = 10000,
		.bits_per_word = 8,
		.cs_change = 0,
	};


	while(1) {
		data[0] = 0x01;								//start bit
		data[1] = 0x10;								//sets CH2 as desired read
		data[2] = 0x00;								//padding for output
		result = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi);
		if (result < 0) {
			perror("ioctl2");
			return 1;
		}
		result = (data[1] << 8) | data[2];
		printf("Value: %d\n", result);
	}
		close(spi_fd);
		return 0;	
} 
