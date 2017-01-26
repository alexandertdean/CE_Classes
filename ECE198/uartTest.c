#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main()
{
	int uart0_filestream = -1;
	struct termios options;
	unsigned char rx_buffer[256];
	int rx_length;
	uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY);
	if (uart0_filestream == -1) {
		perror("Open");
		return 1;
	}
	if (tcgetattr(uart0_filestream, &options)) {
		perror("Getting config");
		return 1;
	}
	options.c_cflag = B4800 | CS8 | CREAD | CLOCAL;
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = ICANON;
	options.c_cc[VMIN] = 0;
	options.c_cc[VTIME] = 0;
	options.c_ospeed = 4800;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);

	while(1) {
	rx_length = read(uart0_filestream, rx_buffer, 255);
	if (rx_length < 0) {
		perror("read");
		return 1;
	}
	rx_buffer[rx_length] = 0;
	printf("%s : %d\n", rx_buffer, rx_length);
	}
	close(uart0_filestream);
	return 0;
}
