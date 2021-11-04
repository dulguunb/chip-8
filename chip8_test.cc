#include <gtest/gtest.h>
#include "chip8.h"
#include <vector>
#include <iostream>
namespace{
  TEST(registerTest,assignmentToV0){
    std::vector<unsigned char> rom;
    rom.push_back(0x60);
    rom.push_back(0x0a);
    chip8 cpu;
    cpu.loadGameFromBuffer(rom);
    cpu.emulateCycle();
    EXPECT_TRUE(cpu.getRegister(0) == 0xA);
  }
  TEST(registerTest,assignmentToV1){
    std::vector<unsigned char> rom;
    rom.push_back(0x61);
    rom.push_back(0x0a);
    chip8 cpu;
    cpu.loadGameFromBuffer(rom);
    cpu.emulateCycle();
    cpu.dumpRegisters();
    EXPECT_TRUE(cpu.getRegister(1) == 0xA);
  }
}
