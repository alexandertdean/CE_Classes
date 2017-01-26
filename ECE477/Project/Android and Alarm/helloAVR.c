#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <util/delay.h>
//#include <util/setbaud.h>
#define F_CPU 1000000UL

void UART_init(void);
char serial_getchar(FILE *);
int serial_putchar(char, FILE *);
static FILE serial_stream = FDEV_SETUP_STREAM (serial_putchar, serial_getchar, _FDEV_SETUP_RW);
void checkWord(void);

int main(void)
{
	UART_init();
	_delay_ms(2000);												//DELAY TO ENSURE COMPLETE INITIALIZATION
 	while(1)
  	{
		checkWord();
  	}
	return 0;
}

void UART_init(void)
{
	UBRR0H = 0;
	UBRR0L = 12;													//BAUD RATE SET TO 9600
	UCSR0A = 1 << U2X0;
	UCSR0C = (1 << USBS0) | (3 << UCSZ00);		//8 BIT DATA, NO PARITY, 2 STOP BITS
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);			//TX AND RX ARE 8 BITS
	stdin = &serial_stream;										
	stdout = &serial_stream;
}

void checkWord(void)
{
	FILE *in;
	FILE *out;
	char word[80];
	int i;
	
	in = stdin;
	out = stdout;


	DDRC = (1 << DDC5);										//ENABLE PORT C, PIN 5
	PORTC = 0x00;
	PORTC |= _BV(PC5); // Turn LED off to start

	DDRC = (1 << DDC4);
	PORTC &= ~_BV(PC4); // Turn LED off to start

	DDRC = (1 << DDC3);
	PORTC &= ~_BV(PC3); // Turn LED off to start

	while(1) {
		for (i = 0; i < 80; i++) {								//MAX LENGTH A WORD CAN BE IS 80 CHARACTERS, RESTART IF LONGER
			while (fscanf(in, "%c", &word[i]) != 1) {		//WAIT FOR VALID CHARACTER TO RECEIVE
				fscanf(in, "%*s");
			}
			if (i == 80) {												//RESTARTS LOOP IF WORD IS TOO LONG
				break;
			}	
			if ((word[i] == ' ') | (word[i] == '.')) {			//WORD IS OVER IF A SPACE OR PERIOD IS RECEIVED
				if (strncmp(word, "red", 3) == 0) {
					DDRC = (1 << DDC5);
					PORTC &= ~_BV(PC5);						//DRIVES PC5 LOW, TURNING ON LED
						//DRIVES PC5 HIGH, TURNING OFF LED
				}
				else if (strncmp(word, "yellow", 3) == 0) {
					DDRC = (1 << DDC4);
					PORTC &= ~_BV(PC4);							//DRIVES PC5 HIGH, TURNING OFF LED
				}
				else if (strncmp(word, "green", 3) == 0) {
					DDRC = (1 << DDC3);
					PORTC &= ~_BV(PC3);							//DRIVES PC5 HIGH, TURNING OFF LED
				}
				break;
			}
		}
	}
}

int serial_putchar(char val, FILE *fp)
{ 
	while((UCSR0A & (1 << UDRE0)) == 0);			//WAITS FOR TX LINE TO BE CLEAR
	UDR0 = val;														//SETS DATA LINE TO VALUE TO SEND
	return 0;															
}

char serial_getchar(FILE *fp)
{
	while ((UCSR0A & (1 << RXC0)) == 0);				//WAITS FOR NEW CHARACTER
	return UDR0;													//RETURNS VALUE RECEIVED BY UART
}	