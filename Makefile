CC = arm-linux-gnueabihf-gcc
C = gcc
CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror
PUBDIR = $(HOME)/cmpt433/public/myApps/

all: clean main noworky
	cp main $(PUBDIR)
	cp noworky $(PUBDIR)

noworky.o: 
	$(CC) $(CFLAGS) -c -g noworky.c

button.o: button.c
	$(CC) $(CFLAGS) -c -g button.c

sampleA2D.o: sampleA2D.c
	$(CC) $(CFLAGS) -c -g sampleA2D.c

sampleAnalyzer.o: sampleAnalyzer.c
	$(CC) $(CFLAGS) -c -g sampleAnalyzer.c

joystick.o: joystick.c
	$(CC) $(CFLAGS) -c -g joystick.c

ledmatrix.o: ledmatrix.c
	$(CC) $(CFLAGS) -c -g ledmatrix.c
	
interface.o: interface.c
	$(CC) $(CFLAGS) -c -g interface.c

main: main.c
	$(CC) $(CFLAGS) -o main -pthread main.c button.c sampleA2D.c sampleAnalyzer.c joystick.c ledmatrix.c interface.c -lm

noworky: noworky.c
	$(CC) $(CFLAGS) -o noworky noworky.c -lm

clean:
	rm -rf *.o main noworky *~
