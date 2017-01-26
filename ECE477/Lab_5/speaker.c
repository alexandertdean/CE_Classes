#include <sys/io.h>
#include <stdio.h>
#include <unistd.h>

int timer_init(void);
int speaker_init(void);

int main() 
{
	int errChk;
	errChk = timer_init();
	if (errChk != 0) {
		perror("timer_init");
		return 1;
	}
	errChk = speaker_init();
	if (errChk != 0) {
		perror("speaker_init");
		return 1;
	}
	outb(0x97, 0x42);
	outb(0x0A, 0x42);
	while(1) {
		outb(inb(0x61) | 0x3, 0x61);
		printf("Out2: %x\nControl: %x\nGate2: %x\nCount: %x\n\n", inb(0x62), inb(0x43), inb(0x61), inb(0x42));
		sleep(10);
		outb(inb(0x61) & ~0x3, 0x61);
		sleep(1);
	}
	return 0;
}

int timer_init(void)
{
	int errChk;
	errChk  = ioperm(0x43, 8, 1);
	if (errChk < 0) {
		perror("ioperm");
		return 1;
	}
	outb(182, 0x43);
	return 0;
}

int speaker_init(void)
{
	int errChk;
	errChk = ioperm(0x42, 8, 1);
	if (errChk < 0) {
		perror("ioperm");
		return 1;
	}
	errChk = ioperm(0x61, 2, 1);
	if (errChk < 0) {
		perror("ioperm");
		return 1;
	}
	return 0;
}
