# Makefile for "/home/svorkoetter/bin/piblank".
# Generated automatically by makemf with the command line:
# makemf -O /home/svorkoetter/bin/piblank
# Do not edit this file!

TARGET = /home/svorkoetter/bin/piblank
CC = gcc
CCFLAGS = -c -I$$HOME/include -O
LD = gcc
LDFLAGS =  -L$$HOME/lib
LIBS = -lm

LIBS += -lX11 -lXss
CCFLAGS += -std=c99 -O3 -Wall -Wno-parentheses -Wno-char-subscripts

$(TARGET): main.o
	$(LD) $(LDFLAGS) -o $(TARGET) *.o $(LIBS)
	strip $(TARGET)

main.o: main.c
	$(CC) $(CCFLAGS) main.c

clean:
	rm -f main.o
