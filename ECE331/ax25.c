#include <linux/module.h>
#include <linux/stat.h>
#include "ax25.h"
#include "crc.h"


int makeax25(char *info, char *callsign, ax25 *input, int len)
{
	int i;						//iterator for for loop
	for (i = 0; i < len; i++) {			//sets info field of struct to inputted info
		input->info[i] = info[i];
	}
	input->control = 0x03f0;			//sets control value to that specified in Homework 6 
	input->fcs = crc((uint8_t *)info, len);	//calculates CRC/FCS based on passed info
	input->address[0] = 'A';			//sets destination address to APZUME with SSID of 0
	input->address[1] = 'P';
	input->address[2] = 'Z';
	input->address[3] = 'U';
	input->address[4] = 'M';
	input->address[5] = 'E';
	input->address[6] = 0;
	input->address[7] = callsign[0];		//sets source address to callsign with SSID of 11
	input->address[8] = callsign[1];
	input->address[9] = callsign[2];
	input->address[10] = callsign[3];
	input->address[11] = callsign[4];
	input->address[12] = callsign[5];
	input->address[13] = 11;
	input->address[14] = 'W' | 0x80;		//sets Layer 2 Repeater address to WIDE2, last address extension bit is 1 as per Homework 6 instructions, SSID of 1
	input->address[15] = 'I';
	input->address[16] = 'D';
	input->address[17] = 'E';
	input->address[18] = '2';
	input->address[19] = ' ';
	input->address[20] = 1;
	return 0;
}
