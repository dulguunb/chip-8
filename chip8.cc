#include "chip8.h"
chip8::chip8(const std::string &fname){
    initialize();
    filename = fname;
}
chip8::chip8(){
    initialize();
}
const unsigned char& chip8::getRegister(const int &i){
    return V[i];
}
void chip8::initialize(){
    memory.reserve(9000);
    V.reserve(16);
    pc = 0x200; // program counter starts at 0x200
    opcode = 0; // Reset current opcode
    I = 0;  // reset index registers
    sp = 0; // reset stack registers
    keyPress = false;
    // Clear display
    for(int i = 0; i < 2048; ++i)
        gfx[i] = 0;

    // Clear stack
    for(int i = 0; i < 16; ++i)
        stack[i] = 0;

    for(int i = 0; i < 16; ++i)
        key[i] = V[i] = 0;

    // Clear memory
    for(int i = 0; i < 4096; ++i)
        memory[i] = 0;

    // Load fontset
    for(int i = 0; i < 80; ++i)
        memory[i] = chip8_fontset[i];		

    // Reset timers
    delay_timer = 0;
    sound_timer = 0;

    // Clear screen once
    drawFlag = true;

    srand (time(NULL));
    for (int i=0;i<80;i++){
        memory[i] = chip8_fontset[i];
    }
}
void chip8::dumpMemory(){
  for(int i=0x200;i<=pc;i++){
    printf("0x%.1X\n",memory[i]);
  }
}
void chip8::dumpRegisters(){
  for(int i=0;i<16;i++){
    printf("V%d=0x%.1X\n",i,V[i]);
  }
}
bool chip8::finished(){
  if (memory[pc] != 0)
    return false;
  return true;
}
unsigned short chip8::getPC(){
  return pc;
}

const std::vector<unsigned char> chip8::getMemory(){
  return memory;
}

void chip8::emulateCycle(){
    opcode = memory[pc];
    opcode <<= 8;
    opcode |= memory[pc+1];
    switch (opcode & 0xF000)
    {
        case 0xAAA:
            I= opcode & 0x0FFF;
            pc+=2;
            break;
        case 0x000:
            switch (opcode&0x000F){
                case 0x000: //
                    break;
                case 0x00EE: // returns from subroutine
                    break;
                default:
                    printf("Unknown opcode [0x000]: 0x0%X",opcode);
            }
            break;
        // TODO: implement the opcodes
        case 0x1000:
            pc = opcode & 0x0FFF;
            break;
        case 0x2000:{
            stack[sp] = pc;
            ++sp;
            pc = opcode & 0x0FFF;
        }
        break;
        case 0x3000:{
            unsigned short v = opcode & V[(opcode & 0x0F00) >> 8];
            if (v == (opcode & 0x00FF)){
                pc+=4;
            }
            else{
                pc+=2;
            }
        }
        break;
        case 0x4000:{
            unsigned short v = opcode & V[(opcode & 0x0F00) >> 8];
            if (v != (opcode & 0x00FF)){
                pc+=4;
            }
            else{
                pc+=2;
            }
        }
        break;
        case 0x5000:{
            if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]){
                pc+=4;
            }
            else{
                pc+=2;
            }
        }
        break;
        case 0x6000:{
            V[(opcode & 0x0F00) >> 8 ] = opcode & 0x00FF;
            pc+=2;
        }
        break;
        case 0x7000:{
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            pc+=2;
        }
        break;
        case 0x8000:{ // Register Operations
            switch(opcode & 0x000F){
                case 0x0000:{
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    pc+=2;
                }
                break;
                case 0x0001:{
                    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                    pc+=2;
                }
                break;
                case 0x0002:{
                    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                    pc+=2;
                }
                break;
                case 0x0003:{
                    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                    pc+=2;
                }
                break;
                case 0x0004:{
                    if (V[(opcode & 0x00F0) >> 4 ] > (0xFF - V[opcode & 0x0F00 >> 8 ] )){
                        V[0xF] = 1;
                    }
                    else{
                        V[0xF] = 0;
                    }
                    V[ (opcode & 0x0F00) >> 8 ] += V[ (opcode & 0x00F0) >> 4 ];
                    pc+=2;
                }
                break;
                case 0x0005:{
                    if (V[(opcode & 0x00F0) >> 4 ] < (0xFF - V[opcode & 0x0F00 >> 8 ] )){
                        V[0xF] = 0;
                    }
                    else{
                        V[0xF] = 1;
                    }
                    V[(opcode & 0x0F00) >> 8]-=V[(opcode & 0x00F0) >> 4];
                    pc+=2;
                }
                break;
                case 0x0006:{
                    unsigned short vxLeastSig = V[(opcode & 0x0F00) >> 8] & 0x1;
                    V[0xF] = vxLeastSig;
                    V[(opcode & 0x0F00) >> 8] >>=1;
                    pc+=2;
                }
                break;
                case 0x0007:{
                    unsigned short vx = V[(opcode & 0x0F00) >> 8];
                    unsigned short vy = V[(opcode & 0x00F0) >> 4];
                    if (vy < vx){
                        V[0xF] = 0;
                    }
                    else{
                        V[0xF] = 1;
                    }
                    V[(opcode & 0x0F00) >> 8] = vy - vx;
                }
                break;
                case 0x000E:{
                    unsigned short vxMostSig = V[(opcode & 0x0F00) >> 8] >> 7;
                    V[0xF] = vxMostSig;
                    V[(opcode & 0x0F00) >> 8] <<=1;
                    pc+=2;
                }
                break;
            }
        }
        break;
        case 0x9000:{
            unsigned short vx = V[(opcode & 0x0F00) >> 8];
            unsigned short vy = V[(opcode & 0x00F0) >> 4];
            if (vx != vy){
                pc+=4;
            }
            else{
                pc+=2;
            }
        }
        break;
        case 0xA000:{
            I = opcode & 0x0FFF;
            pc+=2;
        }
        break;
        case 0xB000:{
            pc = (opcode & 0x0FFF) + V[0];
        }
        break;
        case 0xC000:{
            unsigned short rand_ = rand() % 255;
            V[(opcode & 0x0F00) >> 8] = rand_ & (opcode & 0x00FF);
        }
        break;
        case 0xD000:
        {
        // solution taken from 
        // https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
            unsigned short x = V[(opcode & 0x0F00) >> 8];
            unsigned short y = V[(opcode & 0x00F0) >> 4];
            unsigned short height = opcode & 0x000F;
            unsigned short pixel;
            V[0xF] = 0; // reset the VF
            for(int yline=0;yline<height;yline++){
                pixel = memory[I + yline];
                for(int xline=0;xline<8;xline++){
                    if ((pixel & (0x80 >> xline)) != 0)
                    {
                        if (gfx[(x+xline + ((y + yline) * 64))] == 1){
                            V[0xF] = 1;
                        }
                        gfx[x + xline + ((y+yline) * 64)] ^= 1;
                    }
                }
            }
            drawFlag = true;
            pc+=2;
        }
        break;
        case 0xE000:{
            switch ( opcode & 0x00FF ){
                case 0x009E:{
                    if (key[V[(opcode & 0x0F00) >> 8]] != 0)
                        pc+=4;
                    else
                        pc+=2;
                    }
                }
                break;
                case 0x00A1:{
                    if (key[V[(opcode & 0x0F00) >> 8]] == 0 ){
                        pc+=4;
                    }
                    else{
                        pc+=2;
                    }
                }
                break;
        }
        break;
        case 0xF000:{
            switch(opcode & 0x00FF){
                case 0x0007:{
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                    pc+=2;
                    break;
                }
                case 0x000A:{
                    keyPress = true;
                    for(int i=0;i<16;i++){
                        if(key[i] != 0){
                            V[(opcode & 0x0F00) >> 8] = i;
                            keyPress = true;
                        }
                    }
                    if (!keyPress){
                        return;
                    }
                    pc+=2;
                }
                break;
                case 0x0018:{
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                    pc+=2;
                }
                break;
                case 0x001E:{
                    I+=V[(opcode & 0x0F00) >> 8];
                    pc+=2;
                }
                break;
                case 0x0029:{
                    I = V[(opcode & 0x0F00) >> 8] * 0x5;
                    pc+=2;
                }
                break;
                case 0x0033:{ 
                // solution taken from 
                // https://multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
                    memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
                    memory[I+1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
                    memory[I+2] = (V[opcode & 0x0F00] >> 8 % 100) % 10;
                    pc+=2;
                }
                break;
                case 0x0055: {
                    for(int i=0;i<=(( opcode & 0x0F00 ) >> 8);i++){
                        memory[I+i] = V[i];
                    }
                    I += ((opcode & 0x0F00) >> 8) + 1;
                    pc+=2;
                }
                break;
                case 0x0065: {
                    for(int i=0;i<=(( opcode & 0x0F00 ) >> 8);i++){
                        V[i] = memory[i+I];
                    }
                    I += ((opcode & 0x0F00) >> 8) + 1;
                    pc+=2;
                }
                break;
            }
            break;
        }
        default:
            printf("Unknown opcode: 0x%X\n",opcode);
    }

    if (delay_timer > 0)
        --delay_timer;

    if(sound_timer > 0)
    {
        if(sound_timer == 1)
            printf("BEEP\n");
        --sound_timer;
    }
}

void chip8::loadGameFromFile(const std::string &filename){
    initialize();
    std::ifstream input(filename,std::ios::binary);
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input),{});
    for(int i=0;i<buffer.size();i++){
        memory[i+512] = buffer[i];
    }
    input.close();
}

void chip8::loadGameFromBuffer(const std::vector<unsigned char> &gameRom){
    initialize();
    for(int i=0;i<gameRom.size();i++){
        memory[i+512] = gameRom[i];
    }
}
