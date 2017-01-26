TARGET=speaker
OBJS=sound.o speaker.o
CFLAGS=-g -Wall
CC=gcc

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) 

clean:
	rm -rf $(TARGET) $(OBJS)

.PHONY: clean all
