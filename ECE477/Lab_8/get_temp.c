#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define F_CPU 1000000UL
#include <util/delay.h>

// Simplest possible putchar, waits until UDR is empty and puts character
int serial_putchar(char, FILE *);
// Simplest possible getchar, waits until a char is available and reads it
int serial_getchar(FILE *);
// Sets up File Serial stream variables
static FILE serial_stream = FDEV_SETUP_STREAM (serial_putchar, serial_getchar, _FDEV_SETUP_RW);

// Initializes Mode, ADC, and MUX of the AVR
void setup_sensor(void);
// Initializes the AVR USART for a 9600 baud for 1MHz clock
void setup_serial(void);
// Read the Temperature Sensor of the AVR and output the voltage to TX
void read_temp(void);

int main(void)
{
	// Counter in for loop
	int i;
	// Initialze the temperature sensor in AVR
 	setup_sensor();
	// Initialize the serial communication in AVR
 	setup_serial();

 	while(1)  // Continuously read temperature sensor
 {
    read_temp();   // Read temperature and output voltage to standard out
 }
 return 0;  //never get here
}

// Initializes the AVR USART (Universal Synchronous/Asynchronous Receiver/Transmitter)
void setup_serial(void)
{
   UBRR0H=0;
   UBRR0L=12;                       // Set baud to 9600 for the 1MHz system clock
   UCSR0A= 1<<U2X0;
   UCSR0C= (1<<USBS0)|(3<<UCSZ00) ; // 8 BIT NO PARITY 2 STOP
   UCSR0B=(1<<RXEN0)|(1<<TXEN0)  ;  // Enable 8 bit transmit TX and recieve RX
   stdin=&serial_stream;            // Set the standard input stream
   stdout=&serial_stream;           // Set the standard output stream
}
// Initializes Mode, ADC, and MUX of the AVR
void setup_sensor(void)
{
	// PAGES 247 - 250 IN FULL DATASHEET HELPED WITH INITIALIZATIONS

  /* Set a 1.1V reference, set output to read top 2 bits of ADCH and all 8 bits of ADCL,
	 * set ADC channel 8 to select the temperature sensor
	 */
	ADMUX = (1<<REFS0) | (1<<REFS1) | (0<<ADLAR) | (1<<MUX3);
  ADCSRA = (1<<ADEN) | (1<<ADPS2);  // sets ADC conversions with clock division factor of 26
  ADCSRB = 0;                       // Sets the free running mode

}

// Read the Temperature Sensor of the AVR and output the voltage to TX
void read_temp(void)
{
	// Initalize variables
  FILE *fp;                 // File pointer to ouptut to
  fp=stdout;                // Set TX output to standard out
  int offset = 0;           // Temperature offset
  int calibration = 1;      // Temperature calibration constant
  int temperature;          // Holds Temperature

  ADCSRA |= (1<<ADSC);      // Start the conversion
  while(ADCSRA & (1<<ADSC));// Wait for ADC Interrupt flag
  temperature = (((ADCH<<8)|ADCL)-offset)/calibration; //Calculate temperature found on top of page 248
  fprintf(fp, "Temperature in Volatge (mV): %d\n", temperature);
	_delay_ms(1000);          // Delay so there aren't so many outputs
}

// Simplest possible putchar, waits until UDR is empty and puts character
int serial_putchar(char val, FILE * fp)
{
  while((UCSR0A&(1<<UDRE0)) == 0); // WAIT UNTIL EMPTY
   UDR0 = val;
   return 0;
}

// Simplest possible getchar, waits until a char is available and reads it
//note:1) it is a blocking read (will wait forever for a char)
//note:2) if multiple characters come in and are not read, they will be lost
int serial_getchar(FILE * fp)
{
   while((UCSR0A&(1<<RXC0)) == 0);  // WAIT FOR CHAR
   return UDR0;
 }
