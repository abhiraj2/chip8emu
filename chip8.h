#ifndef CHIP8_H
#define CHIP8_H

#include<cstdint>
#include<chrono>
#include<random>
#include<cstdint>
#include<fstream>
#include<iostream>
#include<cstring>


constexpr uint16_t VIDEO_WIDTH = 64;
constexpr uint16_t VIDEO_HEIGHT = 32;
const unsigned int START_ADDR = 0x200;
const unsigned int FONTSET_START_ADDR = 0x50;
const unsigned int FONTSET_SIZE = 80;

class Chip8{
    public:
        Chip8() : randGen(std::chrono::system_clock::now().time_since_epoch().count()) {
            pc = START_ADDR;

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

            for (unsigned int i = 0; i < FONTSET_SIZE; i++) {
                memory[FONTSET_START_ADDR + i] = fontset[i];
            }

            randByte = std::uniform_int_distribution<unsigned int>(0, 255U);

            table[0x0] = &Chip8::Table0;
            table[0x1] = &Chip8::OP_1nnn;
            table[0x2] = &Chip8::OP_2nnn;
            table[0x3] = &Chip8::OP_3xkk;
            table[0x4] = &Chip8::OP_4xkk;
            table[0x5] = &Chip8::OP_5xy0;
            table[0x6] = &Chip8::OP_6xkk;
            table[0x7] = &Chip8::OP_7xkk;
            table[0x8] = &Chip8::Table8;
            table[0x9] = &Chip8::OP_9xy0;
            table[0xA] = &Chip8::OP_Annn;
            table[0xB] = &Chip8::OP_Bnnn;
            table[0xC] = &Chip8::OP_Cxkk;
            table[0xD] = &Chip8::OP_Dxyn;
            table[0xE] = &Chip8::TableE;
            table[0xF] = &Chip8::TableF;

            for (uint8_t idx = 0; idx <= 0xE; idx++) {
                table0[idx] = &Chip8::OP_NULL;
            }
            table0[0x0] = &Chip8::OP_00E0;
            table0[0xE] = &Chip8::OP_00EE;

            for (uint8_t idx = 0; idx <= 0xE; idx++) {
                table8[idx] = &Chip8::OP_NULL;
            }

            table8[0x0] = &Chip8::OP_8xy0;
            table8[0x1] = &Chip8::OP_8xy1;
            table8[0x2] = &Chip8::OP_8xy2;
            table8[0x3] = &Chip8::OP_8xy3;
            table8[0x4] = &Chip8::OP_8xy4;
            table8[0x5] = &Chip8::OP_8xy5;
            table8[0x6] = &Chip8::OP_8xy6;
            table8[0x7] = &Chip8::OP_8xy7;
            table8[0xE] = &Chip8::OP_8xyE;

            for (uint8_t idx = 0; idx <= 0xE; idx++) {
                tableE[idx] = &Chip8::OP_NULL;
            }
            tableE[0x1] = &Chip8::OP_ExA1;
            tableE[0xE] = &Chip8::OP_Ex9E;

            for (uint8_t idx = 0; idx <= 0x65; idx++) {
                tableF[idx] = &Chip8::OP_NULL;
            }
            tableF[0x07] = &Chip8::OP_Fx07;
            tableF[0x0A] = &Chip8::OP_Fx0A;
            tableF[0x15] = &Chip8::OP_Fx15;
            tableF[0x18] = &Chip8::OP_Fx18;
            tableF[0x1E] = &Chip8::OP_Fx1E;
            tableF[0x29] = &Chip8::OP_Fx29;
            tableF[0x33] = &Chip8::OP_Fx33;
            tableF[0x55] = &Chip8::OP_Fx55;
            tableF[0x65] = &Chip8::OP_Fx65;

        }

        uint8_t reg[16]{};
        uint8_t memory[4096]{};
        uint16_t index{};
        uint16_t pc{};
        uint16_t stack[16]{};
        uint8_t sp{};
        uint8_t delayTimer{};
        uint8_t soundTimer{};
        uint8_t keypad[16]{};
        uint32_t video[VIDEO_WIDTH*VIDEO_HEIGHT]{};
        uint16_t opcode{};

        std::default_random_engine randGen;
        std::uniform_int_distribution<unsigned int> randByte;

        typedef void (Chip8::*Chip8func)(); // this makes Chip8func as a keyword to init function pointers of return type void and no params
        Chip8func table[0xF + 1];
        Chip8func table0[0xE + 1];
        Chip8func table8[0xE + 1];
        Chip8func tableE[0xE + 1];
        Chip8func tableF[0x65 + 1];


        void Table0();
        void Table8();
        void TableE();
        void TableF();
        
        void OP_NULL(){}

        void LoadROM(const char* filepath);
        void OP_00E0(); //CLR
        void OP_00EE(); //RET
        void OP_1nnn(); //JMP
        void OP_2nnn(); //CALL
        void OP_3xkk(); //Skip next instr if Vx = kk  SE Vx byte
        void OP_4xkk(); // SNE Vx byte
        void OP_5xy0(); // SE Vx Vy
        void OP_6xkk(); // LD Vx kk
        void OP_7xkk(); // ADD Vx, byte
        void OP_8xy0(); // LD Vx, Vy
        void OP_8xy1(); // OR Vx, Vy
        void OP_8xy2(); // AND Vx, Vy
        void OP_8xy3(); // XOR Vx, Vy
        void OP_8xy4(); // ADD Vx, Vy
        void OP_8xy5(); // SUB Vx, Vy
        void OP_8xy6(); // SHR Vx
        void OP_8xy7(); // SUBN Vx, Vy
        void OP_8xyE(); // SHL Vx
        void OP_9xy0(); // SNE Vx, Vy
        void OP_Annn(); // LD I, addr
        void OP_Bnnn(); // JP V0, addr
        void OP_Cxkk(); // RND Vx, byte
        void OP_Dxyn(); // DRW Vx, Vy, nibble
        void OP_Ex9E(); // SKP Vx
        void OP_ExA1(); // SKNP Vx
        void OP_Fx07(); // LD Vx DT
        void OP_Fx0A(); // LD Vx, K
        void OP_Fx15(); // LD DT, Vx
        void OP_Fx18(); // LD ST, Vx
        void OP_Fx1E(); // ADD I, Vx
        void OP_Fx29(); // LD F, Vx
        void OP_Fx33(); // LD B, Vx
        void OP_Fx55(); // LD [I], Vx
        void OP_Fx65(); // LD Vx, [I]

        void Cycle();
};

#endif //!CHIP8_H