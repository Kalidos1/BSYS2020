CC = gcc -o
CFLAGS = -g -W -Wall -Wextra -Wpedantic -std=$(STD) -pedantic
STD = gnu17
RM = rm -f

TARGET = mem

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(TARGET) $(TARGET).o

mem: mem.c
	$(CC) mem mem.c $(CFLAGS)