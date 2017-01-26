#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
 
/*
 * 'open_port()' - Open serial port 1.
 *
 * Returns the file descriptor on success or -1 on error.
 */
 
int main()
{
  int fd;  // File descriptor
  int n;
 
  fd = open_port();
 
  // Read the configureation of the port 
 
  struct termios options;
  tcgetattr( fd, &options );
 
  /* SEt Baud Rate */
 
  cfsetispeed( &options, B9600 );
  cfsetospeed( &options, B9600 );
  
  //I don't know what this is exactly 
 
  options.c_cflag |= ( CLOCAL | CREAD );
 
  // Set the Charactor size
 
  options.c_cflag &= ~CSIZE; /* Mask the character size bits */
  options.c_cflag |= CS8;    /* Select 8 data bits */
 
  // Set parity - No Parity (8N1)
   
  options.c_cflag &= ~PARENB;
  options.c_cflag &= ~CSTOPB;
  options.c_cflag &= ~CSIZE;
  options.c_cflag |= CS8;
 
  // Disable Hardware flowcontrol
 
  //  options.c_cflag &= ~CNEW_RTSCTS;  -- not supported
 
  // Enable Raw Input
   
  options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
 
  // Disable Software Flow control
 
  options.c_iflag &= ~(IXON | IXOFF | IXANY);
 
  // Chose raw (not processed) output
 
  options.c_oflag &= ~OPOST;
 
  if ( tcsetattr( fd, TCSANOW, &options ) == -1 )
    printf ("Error with tcsetattr = %s\n", strerror ( errno ) );
  else
    printf ( "%s\n", "tcsetattr succeed" );
 
  fcntl(fd, F_SETFL, FNDELAY);
 
 
  // Write some stuff !!!
 
  n = write(fd, "ATZ\r", 4);
  if (n < 0)
    fputs("write() of 4 bytes failed!\n", stderr);
  else
    printf ("Write succeed n = %i\n", n );
 
  sleep(5);
 
  char buf;
 
  n = read( fd, &buf, 1 );
  printf ( "Buf = %c\n", buf );
 
  if ( n == -1 )
      printf ( "Error = %s\n", strerror( errno ) );
 

 
  close( fd );
  return 0;
 
}
 
       
 
int open_port(void)
{
  int fd; /* File descriptor for the port */
 
  fd = open("/dev/ttyUSB1", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1)
    {
      /*
       * Could not open the port.
       */
 
      perror("open_port: Unable to open /dev/ttyS0 - ");
    }
  else
    fcntl(fd, F_SETFL, FNDELAY);
 
  printf ( "In Open port fd = %i\n", fd);
 
  return (fd);
}
