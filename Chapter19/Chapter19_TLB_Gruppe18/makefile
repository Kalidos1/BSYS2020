CC = gcc -o
CFLAGS = -g -W -Wall -Wextra -Wpedantic -std=$(STD) -pedantic
STD = gnu17
RM = rm -f

TARGET = tlb

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(TARGET) $(TARGET).o

tlb: tlb.c
	$(CC) tlb tlb.c $(CFLAGS)