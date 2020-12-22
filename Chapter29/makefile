CC = gcc -o
CFLAGS = -g -W -Wall -Wextra -Wpedantic -std=$(STD) -pedantic -pthread
STD = gnu17
RM = rm -f

TARGET = concurrentCounter sloppyCounter linkedList hashList bTree

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(TARGET) $(TARGET).o

concurrentCounter: concurrentCounter.c mythreads.h
	$(CC) concurrentCounter concurrentCounter.c $(CFLAGS)

sloppyCounter: sloppyCounter.c mythreads.h
	$(CC) sloppyCounter sloppyCounter.c $(CFLAGS)

linkedList: linkedList.c mythreads.h
	$(CC) linkedList linkedList.c $(CFLAGS)

hashList: hashList.c mythreads.h
	$(CC) hashList hashList.c $(CFLAGS)

bTree: bTree.c mythreads.h
	$(CC) bTree bTree.c $(CFLAGS)