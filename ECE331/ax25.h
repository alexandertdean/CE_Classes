#ifndef __AX25
#define __AX25

typedef struct ax25{
	uint8_t *address;
	uint16_t control;
	char *info;
	uint16_t fcs;
} ax25;

int makeax25(char *info, char *callsign, ax25 *input, int len);

#endif
 
