TARGET = band
OBJS = rem.o u2.o ccr.o kiss.o
CC = gcc
CFLAGS = -Wall -g
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: clean all
