#include <gtest/gtest.h>
#include "chip8.h"
#include <vector>
#include <iostream>
namespace{
  TEST(opcode,_0x0000){
    std::vector<unsigned char> rom;
    rom.push_back(0x00);
    rom.push_back(0x0a);
    chip8 cpu;
  }
  TEST(opcode,_0x1000){
    std::vector<unsigned char> rom;
    rom.push_back(0x12);
    rom.push_back(0x01);
    chip8 cpu;
    cpu.loadGameFromBuffer(rom);
    cpu.emulateCycle();
    EXPECT_TRUE(cpu.getPC() == 0x201);
    rom.clear();
    rom.push_back(0x12);
    rom.push_back(0x13);
    cpu.loadGameFromBuffer(rom);
    cpu.emulateCycle();
    EXPECT_TRUE(cpu.getPC() == 0x213);
    EXPECT_FALSE(cpu.getPC() == 0x201);
  }
  TEST(opcode,_0x2000){
    std::vector<unsigned char> rom;
  }
  TEST(opcode,_0x6000){
    for(int i=0;i<16;i++){
      std::vector<unsigned char> rom;
      rom.push_back(0x60+i);
      rom.push_back(0x0a);
      chip8 cpu;
      cpu.loadGameFromBuffer(rom);
      cpu.emulateCycle();
      EXPECT_TRUE(cpu.getRegister(i) == 0xA);
      EXPECT_TRUE(cpu.getPC() == (0x200 + 0x2));
    }
  }
}
