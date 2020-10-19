CC = gcc -o
CFLAGS = -g -W -Wall -Wextra -Wpedantic -std=$(STD) -pedantic
STD = gnu17
RM = rm -f

TARGET = systemCall #contextSwitch

.PHONY: all clean

all: $(TARGET)

clean:
	$(RM) $(TARGET) $(TARGET).o

systemCall: systemCall.c
	$(CC) systemCall systemCall.c $(CFLAGS)

#contextSwitch: contextSwitch.c
#	$(CC) contextSwitch contextSwitch.c $(CFLAGS)