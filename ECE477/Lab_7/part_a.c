#include <avr/io.h>
#include <avr/delay.h>

int main(void)
{
	//DDRD = 0xFF; // for pin 2 RXD
	int x = 0;
	while(1)
	{
		x = 1;
		do{
			PORTD = 0x01;
			_delay_ms(500);
			PORTD = 0x00;
			_delay_ms(500);
		} while(x=1);
	}
}
