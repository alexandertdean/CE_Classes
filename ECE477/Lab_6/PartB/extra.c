#include <stdio.h>
#include <sys/io.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

void setPermission(void);
int serialOut(void);
int getTimerVal(int port);

int main(){
	int errChk;
	errChk = permission();										// Set up permissions
	if (errChk == 1) {
		return 1;
	}
	fd = open("/dev/ttyUSB0", O_RDWR); 							//Open serial port as read/write
	if(fd == NULL){	
		perror("open");											//Cannot open port, throw error
	}
	errChk = serialOut();										// Output to serial port
	return errChk;
}

int setPermission(){
	int control_byte = 0b10110100;								//sets mode and other settings of timer
	if(ioperm(0x43,1,1)!=0) { 									// Allows permission to access the port 67
		perror("ioperm (port 67)") 		//Check permission
		return 1;
	}
	outb(control_byte,0x43); 									//Set 10110100b to port 67 to set mode of timer 
	if(ioperm(0x42,1,1)!=0) {									// Allows permission to access the port 66
		perror("ioperm (port 66)"); 							//Check permission
		return 1;
	}
}

int sserialOut(){
	int threshold, timer_value, serial;
	int mask = TIOCM_DTR;
	int timer_start_val = 4000;									//Value timer starts counting down at, initialized to value from Part A
	char upper_start_val;										//upper 8 bits of timer_start_val to pass by calling outb()  (second-least significant bit)
	char lower_start_val;										//lower 8 bits of timer_start_val to pass by calling outb()
	
	fd = open("/dev/ttyUSB0", O_RDWR); 							//Open serial port as read/write
	if(fd == NULL){	
		perror("open");											//Cannot open port, throw error
		return 1;
	}
	
	while(1){
		upper_start_val = (timer_start_val & 0x0000FF00) >> 8;	//masks timer_start_val to select second-least significant byte
		lower_start_val = (timer_start_val & 0x000000FF);		//masks timer_start_val to select least significant byte
		
		outb(lower_start_val, 0x42); 							//Sets lower byte of timer start value
		outb(upper_start_val, 0x42); 							//Sets upper byte of timer start vlaue
		
		ioctl(fd, TIOCMGET, &serial); 							//fetches current levels of each pin of FT232RL chip.
		threshold = timer_start_val / 2;
		
		timer_value = getTimerVal(0x42);						//gets current value of timer
		
		if(timer_value <= threshold) {							// Compare timer to threshold and set or clear bits, desired output pin (DTR) goes either high or low.
			ioctl(fd, TIOCMSET, &mask); 						// sets bit of DTR pin
		} else {
		  ioctl(fd, TIOCMBIC, &mask);  							// clears bit of DTR pin
		}
		if (timer_start_val < 2500) {
			timer_start_val = 5000;								//resets timer start value to keep sound from going too high pitched
		} else {
			timer_start_val -= 1;								//decrements timer start value, increasing the frequencyo of the sound
		}
	}
	close(fd);
	return 0;
}

int getTimerVal(int port){
	int lower_bits = 0;
	lower_bits = inb(port); 									// Get lower bits on port 0x42
	lower_bits |= ((inb(port)<<8)&0xFF00);						// Get upper bits on port 0x42
	return lower_bits;
}


