CC = gcc -o
CFLAGS = -g -W -Wall -Wextra -Wpedantic -std=$(STD) -pedantic -pthread
STD = gnu17
RM = rm -f

TARGET = concurrentCounter

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(TARGET) $(TARGET).o

concurrentCounter: concurrentCounter.c mythreads.h
	$(CC) concurrentCounter concurrentCounter.c $(CFLAGS)