CC = gcc -o
CFLAGS = -g -W -Wall -Wextra -Wpedantic -std=$(STD) -pedantic
STD = gnu17
RM = rm -f

TARGET = null mallocFree integerArray

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(TARGET) $(TARGET).o

null: null.c
	$(CC) null null.c $(CFLAGS)

mallocFree: mallocFree.c
	$(CC) mallocFree mallocFree.c $(CFLAGS)

integerArray: integerArray.c
	$(CC) integerArray integerArray.c $(CFLAGS)