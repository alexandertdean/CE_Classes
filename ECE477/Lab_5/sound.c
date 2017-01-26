#include <sys/io.h>
#include "sound.h"

int read_timer(unsigned int channel)
{
	if (channel > 2) {
		return 0xffff;
	}
	outb(channel << 6, 0x42);		//latch
	return inb(0x40 + channel) + (inb(0x40 + channel) << 8);
}

void sound (long freq)
{
	freq = DIVISOR(freq);
	//assumes that the channel has been initialized with a 0xb6

	outb(0xb, 0x42);
	outb((freq & 0xff), 0x42);
	outb(((freq >> 8) & 0xff), 0x42);
}

void soundoff(void) {
	outb(inb(0x61) & ~3, 0x61);
}

void soundon(void) {
	outb((inb(0x61) | 3), 0x61);
}
