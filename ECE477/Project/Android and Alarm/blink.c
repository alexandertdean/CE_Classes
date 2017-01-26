#include <stdio.h>
#include <sys/io.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

void permission(void);
void set_serial(int fd);
int get_bits(int port);

void main(){
  int fd;
  // Set up permissions
  permission();
  // Open serial port
  fd = open("/dev/ttyUSB0", O_RDWR); //Open serial port as read/write
  if(fd == -1){
    //Cannot open port, throw error
    printf("Unable to open /dev/ttyUSB0\n");
  }
  // Output to serial port
  set_serial(fd);
}

void permission(){

  int lower_byte = 0b10100000;
  int upper_byte = 0b00001111;
  int control_byte = 0b10110100;
  
  // Allows permission to access the port 0x43
  if(ioperm(0x43,1,1)!=0) 
    printf("Error: Check permissions on port 43\n"); //Check permission
  outb(control_byte,0x43); //Set 10110100b to port 0x43

  // Allows permission to access the port 0x42 
  if(ioperm(0x42,1,1)!=0) 
    printf("Error: Check permissions on port 42\n"); //Check permission
  outb(lower_byte,0x42); //Set 10100000b to port 0x42
  outb(upper_byte,0x42); //Set 00001111b to port 0x42
}

void set_serial(int fd){
  int threshold, timer_value, serial;
  int mask = TIOCM_DTR;
  threshold = 256;
  while(1){
    ioctl(fd, TIOCMGET, &serial);
    /*
    if(serial & TIOCM_RNG) 
      threshold = 3750;
    else 
      threshold = 256;
   
    timer_value = get_bits(0x42);
    */
    // Compare timer to threshold and set or clear bits
    //if(timer_value <= threshold) 
    
    printf("LED ON\n");
    ioctl(fd, TIOCMSET, &mask);  // sets bit of DTR pin
    sleep(1);
    //else 
    printf("LED OFF\n");
    ioctl(fd, TIOCMBIC, &mask);  // clears bit of DTR pin
    // Show user the current threshold 
    printf("Threshold is currently: %d\n", threshold);
  }
  close(fd);
}

int get_bits(int port){
  int lower_bits = 0;
  
  // Get lower bits on port 0x42
  lower_bits = inb(port); 
  // Get upper bits on port 0x42
  lower_bits |= ((inb(port)<<8)&0xFF00);

  return lower_bits;
}