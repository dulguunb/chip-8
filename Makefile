CC=g++

chip8_test: chip8.o
	$(CC) -Wall -pthread -g chip8_test.cc chip8.o -lgtest_main -lgtest -o chip8_test
chip8.o: chip8.h chip8.cc
	$(CC) -g -c chip8.cc -o chip8.o
main.o: main.cc
	$(CC) $(shell pkg-config --libs sdl2) $(shell pkg-config --cflags sdl2) -g -c main.cc -o main.o
all: main.o chip8.o chip8_test
	$(CC) $(shell pkg-config --libs sdl2) $(shell pkg-config --cflags sdl2) -g main.o chip8.o -o main
clean:
	rm -rf chip8.o main.o main chip8_test
