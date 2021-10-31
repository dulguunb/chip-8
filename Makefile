CC=g++
chip8.o: chip8.h chip8.cc
	$(CC) -g -c chip8.cc -o chip8.o
main.o: main.cc
	$(CC) -g -c main.cc -o main.o
all: main.o chip8.o
	$(CC) -g main.o chip8.o -o main
clean:
	rm -rf chip8.o main.o main
