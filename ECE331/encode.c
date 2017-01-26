#include <linux/module.h>
#include <linux/kernel.h>
#include "encode.h"

int encode(char *data, int stuffOn, int len)
{
	char state = 1;
	char oneCount;							//used for counting 1's for bit stuffing
	char bit;							//counts number of bits to know when to increment stuffed array;
	int i;								//for loop iterator variable
	int j;								//iterator for stuffed array
	for (i = 0; i < len; i++) {
		for (j = 0; j < 8; j++) {
			bit = data[i] & (0x01 << j);
			printk(KERN_INFO "%c ", state?'S':'M');
			if (bit != 0) {
				oneCount++;
				if (oneCount == 5 && stuffOn != 0) {	//checks if bit stuffing should occur
					printk(KERN_INFO "M ");			//stuffs a 0
					state = !state;
				}
			} else {
				oneCount = 0;
				state = !state;				//toggles state
			}
			
		}
	}
	printk(KERN_INFO "%c\n", state?'S':'M');
	return 0;
}
