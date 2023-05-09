#include "./chip8.h"
#include<fstream>
#include<iostream>


Chip8::Chip8() : randGen(std::chrono::system_clock::now().time_since_epoch().count()){
    pc = START_ADDR;

    const unsigned int FONTSET_START_ADDR = 0x50;
    const unsigned int FONTSET_SIZE = 80;

    uint8_t fontset[FONTSET_SIZE] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, //0
        0x20, 0x60, 0x20, 0x20, 0x70, //1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
        0x90, 0x90, 0xF0, 0x10, 0x10, //4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
        0xF0, 0x10, 0x20, 0x40, 0x40, //7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
        0xF0, 0x90, 0xF0, 0x90, 0x90, //A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
        0xF0, 0x80, 0x80, 0x80, 0xF0, //C
        0xE0, 0x90, 0x90, 0x90, 0xE0, //D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
        0xF0, 0x80, 0xF0, 0x80, 0x80  //F
    };

    for(unsigned int i=0; i< FONTSET_SIZE; i++){
        memory[FONTSET_START_ADDR + i] = fontset[i];
    }

    randByte = std::uniform_int_distribution<uint8_t>(0, 255U);
}

/*
LoadROM reads a ROM file from the filesystem and loads it in the memory
*/
void Chip8::LoadROM(const char* filepath){
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    
    if(!file.is_open()){
        std::cout << "File Error" << std::endl;
        return;
    }
    
    std::streampos size = file.tellg();
    char* buffer = new char[size];

    file.seekg(0, std::ios::beg);
    file.read(buffer, size);
    file.close();

    for(int i=0; i<size; i++){
        memory[START_ADDR + i] = buffer[i];
    }

    delete[] buffer;
}

void Chip8::OP_00E0(){
    memset(video, 0, sizeof(video));
}

void Chip8::OP_00EE(){
    --sp;
    pc = stack[sp];
}

void Chip8::OP_1nnn(){
    uint16_t address = opcode & 0x0FFFu;
    pc = address;
}

void Chip8::OP_2nnn(){
    uint16_t address = opcode & 0x0FFFu;
    stack[sp++] = pc;
    pc = address; 
}   

void Chip8::OP_3xkk(){
    uint8_t val = opcode & 0x00FF;
    uint8_t reg_idx = (opcode & 0x0F00) >> 8u;

    if(reg[reg_idx] == val){
        pc+= 2;
    }
}

void Chip8::OP_4xkk(){
    uint8_t val = opcode & 0x00FF;
    uint8_t reg_idx = (opcode & 0x0F00) >> 8u;

    if(reg[reg_idx] != val){
        pc+= 2;
    }
}

void Chip8::OP_5xy0(){
    uint8_t reg_idx1 = (opcode & 0x0F00) >> 8u;
    uint8_t reg_idx2 = (opcode & 0x00F0) >> 4u;

    if(reg[reg_idx1] != reg[reg_idx2]){
        pc+= 2;
    }
}

void Chip8::OP_6xkk(){
    uint8_t val = opcode & 0x00FF;
    uint8_t reg_idx = (opcode & 0x0F00) >> 8u;

    reg[reg_idx] = val;
}

void Chip8::OP_7xkk(){
    uint8_t val = opcode & 0x00FF;
    uint8_t reg_idx = (opcode & 0x0F00) >> 8u;

    reg[reg_idx] += val;
}

void Chip8::OP_8xy0(){
    uint8_t reg_idx1 = (opcode & 0x0F00) >> 8u;
    uint8_t reg_idx2 = (opcode & 0x00F0) >> 4u;

    reg[reg_idx1] = reg[reg_idx2];
}

void Chip8::OP_8xy1(){
    uint8_t reg_idx1 = (opcode & 0x0F00) >> 8u;
    uint8_t reg_idx2 = (opcode & 0x00F0) >> 4u;

    reg[reg_idx1] |= reg[reg_idx2];
}

void Chip8::OP_8xy2(){
    uint8_t reg_idx1 = (opcode & 0x0F00) >> 8u;
    uint8_t reg_idx2 = (opcode & 0x00F0) >> 4u;

    reg[reg_idx1] &= reg[reg_idx2];
}

void Chip8::OP_8xy3(){
    uint8_t reg_idx1 = (opcode & 0x0F00) >> 8u;
    uint8_t reg_idx2 = (opcode & 0x00F0) >> 4u;

    reg[reg_idx1] ^= reg[reg_idx2];
}

void Chip8::OP_8xy4(){
    uint8_t reg_idx1 = (opcode & 0x0F00) >> 8u;
    uint8_t reg_idx2 = (opcode & 0x00F0) >> 4u;

    uint16_t sum = reg[reg_idx1] + reg[reg_idx2];

    if(sum > 255u){
        reg[15u] = 1;
    }
    else{
        reg[15u] = 0;
    }
    reg[reg_idx1] = sum & 0xFFu;
}