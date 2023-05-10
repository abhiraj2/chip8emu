#include<cstdint>
#include<chrono>
#include<random>

class Chip8{
    public:
        const unsigned int START_ADDR = 0x200;
        uint8_t reg[16]{};
        uint8_t memory[4096]{};
        uint16_t index{};
        uint16_t pc{};
        uint16_t stack[16]{};
        uint8_t sp{};
        uint8_t delayTimer{};
        uint8_t soundTimer{};
        uint8_t keypad[16]{};
        uint32_t video[64*32]{};
        uint16_t opcode;

        std::default_random_engine randGen;
        std::uniform_int_distribution<uint8_t> randByte;

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
        void OP_8xy8(); // SHL Vx
        void OP_9xy0(); // SNE Vx, Vy
        void OP_Annn(); // LD I, addr
        void OP_Bnnn(); // JP V0, addr
        void OP_Cxkk(); // RND Vx, byte
};
