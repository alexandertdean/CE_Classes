CC=avr-gcc
CFLAGS=-g -Os -Wall -mcall-prologues -mmcu=atmega88
OBJ2HEX=avr-objcopy
TARGET=get_temp
ADFLAGS=-p m88 -c avrispmkII -P usb

.PHONY: fuses prog erase clean


prog : $(TARGET).hex $(TARGET).eeprom
	sudo avrdude $(ADFLAGS) -V -U flash:w:$(TARGET).hex:i


%.obj : %.o
	$(CC) $(CFLAGS) $< -o $@

%.hex : %.obj
	$(OBJ2HEX) -R .eeprom -O ihex $< $@

%.eeprom : %.obj
	$(OBJ2HEX) -j .eeprop -O ihex $< $@

erase :
	sudo /usr/local/bin/avrdude $(ADFLAGS) -E noreset -e
clean :
	rm -f *.hex *.obj *.o

fuses:
	sudo /usr/local/bin/avrdude $(ADFLAGS) -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xf9:m 
# values from http://www.engbedded.com/fusecalc/


