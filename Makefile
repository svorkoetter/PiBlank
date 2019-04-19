TARGET = piblank
CC = gcc
CCFLAGS = -c -I$$HOME/include -std=c99 -O3 -Wall -Wno-parentheses -Wno-char-subscripts
LD = gcc
LIBS = -lm -lX11 -lXss

$(TARGET): main.o
	$(LD) $(LDFLAGS) -o $(TARGET) *.o $(LIBS)
	strip $(TARGET)

main.o: main.c
	$(CC) $(CCFLAGS) main.c

clean:
	rm -f main.o
