#ifndef CHIP_8
#define CHIP_8
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <time.h>
class chip8 {
    private:
        unsigned short pc;
        unsigned short opcode;
        unsigned short I;
        unsigned short sp;
        unsigned short stack[16];
        std::vector<unsigned char> memory;
        int bufferSize;
        std::vector<unsigned char> V; // CPU registers
        unsigned char gfx[64 * 32]; // the screen has pixel of 2048
        unsigned char delay_timer;
        unsigned char sound_timer;
        unsigned char key[16];
        bool keyPress;
        unsigned char chip8_fontset[80] = {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };
        bool drawFlag = false;
        std::string filename;
    public:
        unsigned short getPC();
        const std::vector<unsigned char> getMemory();
        const unsigned char& getRegister(const int &i);
        chip8();
        chip8(const std::string &filename);
        void initialize();
        void emulateCycle(); //fetch Opcode, decode opcode, execute opcode, update timers
        void loadGameFromBuffer(const std::vector<unsigned char> &gameRom);
        void loadGameFromFile(const std::string &filename);
        void dumpMemory();
        void dumpRegisters();
        bool finished();
};


#endif
