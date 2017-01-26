#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main()
{
	int fd;
	int result;
	fd = open("/sys/class/gpio/export", O_RDWR);
	if (fd < 0) {
		perror("Open");
		return 1;
	}
	result = write(fd, "2", 1);
	if (result < 0) {
		perror("Write for GPIO2");
		return 1;
	}
	result = write(fd, "3", 1);
	if (result < 0) {
		perror("Write for GPIO3");
		return 1;
	}
	result = write(fd, "4", 1);
	if (result < 0) {
		perror("Write for GPIO4");
		return 1;
	}
	close(fd);
	fd = open("/sys/class/gpio/gpio2/direction", O_RDWR);
	if (fd < 0) {
		perror("Open for GPIO2 Direction");
		return 1;
	}
	result = write(fd, "OUT", 3);
	if (result < 0) {
		perror("Write for GPIO2 Direction");
		return 1;
	}
	close(fd);
	fd = open("/sys/class/gpio/gpio3/direction", O_RDWR);
	if (fd < 0) {
		perror("Open for GPIO3 Direction");
		return 1;
	}
	result = write(fd, "OUT", 3);
	if (result < 0) {
		perror("Write for GPIO3 Direction");
		return 1;
	}
	close(fd);
	fd = open("/sys/class/gpio/gpio4/direction", O_RDWR);
	if (fd < 0) {
		perror("Open for GPIO4 Direction");
		return 1;
	}
	result = write(fd, "OUT", 3);
	if (result < 0) {
		perror("Write for GPIO4 Direction");
		return 1;
	}
	fd = open("/sys/class/gpio/gpio2/value", O_RDWR);
	if (fd < 0) {
		perror("Open for GPIO2 Value");
		return 1;
	}
	result = write(fd, "1", 1);
	if (result < 0) {
		perror("Write for GPIO2 Value");
		return 1;
	}
	close(fd);
	while(1);
	return 0;
}

