#ifndef __SOUND
#define __SOUND

void soundon(void);
void soundoff(void);
int read_timer(unsigned int channel);
void sound(long int freq);
#define DIVISOR(frequency) ((unsigned int) ((1193181.6666 / frequency) + 0.5))
#endif
