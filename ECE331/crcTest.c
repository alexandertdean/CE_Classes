#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

uint16_t crc(uint8_t *data, int len);

int main(int argc, char *argv[])
{
	uint16_t crcout;					
	if (argc > 2) {						//checks for proper number of arguments
		printf("Usage: %s input", argv[0]);		
		return 1; 
	}
	
	crcout = crc((uint8_t *)argv[1], strlen(argv[1]));	//passes the data and length of data in bytes to crc function
	printf("%4X\n", crcout);				//prints result of CRC calculation
	return 0;
}

uint16_t crc(uint8_t *data, int len)
{	
	uint16_t crc = 0xFFFF;					//CRC register initialized to 0xFF as given in instructions
	uint16_t poly = 0x8408;					//CRC-16-CCITT reverse CRC value
	int byte;						//iterator for for loop that iterates through each byte of data
	int bit;						//iterator for for loop that iterates through each bit of a given byte
	int xorOn;						//feedback value that handles 17th bit 
	for (byte = 0; byte < len; byte++) {
		for (bit = 0; bit < 8; bit++) {
			xorOn = (data[byte] & 1) ^ (crc & 1);	//calculates feedback value
			crc >>= 1;				
			data[byte] >>= 1;			//feeds in next bit of data
			if (xorOn) {				//if feedback is 0, CRC can be shifted with no other operation. if feedback is 1, values at polynomial must be inverted
				crc ^= poly;
			}
		}
	}
	return ~crc;						//returns 1's compliment of calculated value
}
