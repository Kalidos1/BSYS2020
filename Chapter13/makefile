CC = gcc -o
CFLAGS = -g -W -Wall -Wextra -Wpedantic -std=$(STD) -pedantic
STD = gnu17
RM = rm -f

TARGET = memory-user

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(TARGET) $(TARGET).o

memory-user: memory-user.c
	$(CC) memory-user memory-user.c $(CFLAGS)