#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <util/delay.h>
#include <util/setbaud.h>


void UART_init(void);
char serial_getchar(FILE *);
int serial_putchar(char, FILE *);
static FILE serial_stream = FDEV_SETUP_STREAM (serial_putchar, serial_getchar, _FDEV_SETUP_RW);
void blinkNum(num);

int main(void)
{
	FILE *in;
	FILE *out;
	int num;
	
	UART_init();
	_delay_ms(2000);												//DELAY TO ENSURE COMPLETE INITIALIZATION
	DDRC = (1 << DDC5)										//ENABLE PORT C, PIN 5
	PORTC = 0x00;
	
	in = stdin;
	out = stdout;
	
 	while(1)
  	{
		fprintf(out, "Num");										//TRANSMITS STRING WITH UART
		while (fscanf(in, "%d", num) != 1) {				//WAIT FOR VALID NUMBER TO RECEIVE
			fscanf(in, "%*s");
		}
		blinkNum(num);
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

void blinkNum(num)
{
	int i;
	for (i = 0; i < num; i++) {									//BLINKS LED NUM TIMES
		PORTC &= ~_BV(PC5)
		_delay_ms(500);
		PORTC |= _BV(PC5);
		_delay_ms(500);
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