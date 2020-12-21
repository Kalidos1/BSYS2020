CC = gcc -o
CFLAGS = -g -W -Wall -Wextra -Wpedantic -std=$(STD) -pedantic -pthread
STD = gnu17
RM = rm -f

TARGET = concurrentCounter sloppyCounter

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(TARGET) $(TARGET).o

concurrentCounter: concurrentCounter.c mythreads.h
	$(CC) concurrentCounter concurrentCounter.c $(CFLAGS)

sloppyCounter: sloppyCounter.c mythreads.h
	$(CC) sloppyCounter sloppyCounter.c $(CFLAGS)