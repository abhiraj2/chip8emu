#include "chip8.h"

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

    for(long i=0; i<size; i++){
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
    stack[sp] = pc;
    ++sp;
    pc = address; 
}   

void Chip8::OP_3xkk(){
    uint8_t val = opcode & 0x00FFu;
    uint8_t reg_idx = (opcode & 0x0F00u) >> 8u;

    if(reg[reg_idx] == val){
        pc+= 2;
    }
}

void Chip8::OP_4xkk(){
    uint8_t val = opcode & 0x00FFu;
    uint8_t reg_idx = (opcode & 0x0F00u) >> 8u;

    if(reg[reg_idx] != val){
        pc+= 2;
    }
}

void Chip8::OP_5xy0(){
    uint8_t reg_idx1 = (opcode & 0x0F00) >> 8u;
    uint8_t reg_idx2 = (opcode & 0x00F0) >> 4u;

    if(reg[reg_idx1] == reg[reg_idx2]){
        pc+= 2;
    }
}

void Chip8::OP_6xkk(){
    uint8_t val = opcode & 0x00FFu;
    uint8_t reg_idx = (opcode & 0x0F00u) >> 8u;

    reg[reg_idx] = val;
}

void Chip8::OP_7xkk(){
    uint8_t val = opcode & 0x00FFu;
    uint8_t reg_idx = (opcode & 0x0F00u) >> 8u;

    reg[reg_idx] += val;
}

void Chip8::OP_8xy0(){
    uint8_t reg_idx1 = (opcode & 0x0F00u) >> 8u;
    uint8_t reg_idx2 = (opcode & 0x00F0u) >> 4u;

    reg[reg_idx1] = reg[reg_idx2];
}

void Chip8::OP_8xy1(){
    uint8_t reg_idx1 = (opcode & 0x0F00u) >> 8u;
    uint8_t reg_idx2 = (opcode & 0x00F0u) >> 4u;

    reg[reg_idx1] |= reg[reg_idx2];
}

void Chip8::OP_8xy2(){
    uint8_t reg_idx1 = (opcode & 0x0F00u) >> 8u;
    uint8_t reg_idx2 = (opcode & 0x00F0u) >> 4u;

    reg[reg_idx1] &= reg[reg_idx2];
}

void Chip8::OP_8xy3(){
    uint8_t reg_idx1 = (opcode & 0x0F00u) >> 8u;
    uint8_t reg_idx2 = (opcode & 0x00F0u) >> 4u;

    reg[reg_idx1] ^= reg[reg_idx2];
}

void Chip8::OP_8xy4(){
    uint8_t reg_idx1 = (opcode & 0x0F00u) >> 8u;
    uint8_t reg_idx2 = (opcode & 0x00F0u) >> 4u;

    uint16_t sum = reg[reg_idx1] + reg[reg_idx2];

    if(sum > 255U){
        reg[0xF] = 1;
    }
    else{
        reg[0xF] = 0;
    }
    reg[reg_idx1] = sum & 0xFFu;
}

void Chip8::OP_8xy5(){
    uint8_t reg_idx1 = (opcode & 0x0F00) >> 8u;
    uint8_t reg_idx2 = (opcode & 0x00F0) >> 4u;

    if(reg[reg_idx1] > reg[reg_idx2]){
        reg[0xF] = 1;
    }
    else{
        reg[0xF] = 0;
    }
    reg[reg_idx1] -= reg[reg_idx2];
}

void Chip8::OP_8xy6(){
    uint8_t reg_idx1 = (opcode & 0x0F00u) >> 8u;
    reg[0xF] = (reg[reg_idx1] & 0x1u);
    reg[reg_idx1] >>= 1;
}

void Chip8::OP_8xy7(){
    uint8_t reg_idx1 = (opcode & 0x0F00u) >> 8u;
    uint8_t reg_idx2 = (opcode & 0x00F0u) >> 4u;

    if(reg[reg_idx1] < reg[reg_idx2]){
        reg[0xF] = 1;
    }
    else{
        reg[0xF] = 0;
    }
    reg[reg_idx1] = reg[reg_idx2] - reg[reg_idx1];
}

void Chip8::OP_8xyE(){
    uint8_t reg_idx1 = (opcode & 0x0F00u) >> 8u;
    reg[0xF] = (reg[reg_idx1] & 0x80u) >> 7u;
    reg[reg_idx1] <<= 1;
}

void Chip8::OP_9xy0(){
    uint8_t reg_idx1 = (opcode & 0x0F00) >> 8u;
    uint8_t reg_idx2 = (opcode & 0x00F0) >> 4u;

    if(reg[reg_idx1] != reg[reg_idx2]){
        pc += 2;
    }
}

void Chip8::OP_Annn(){
    uint16_t addr = (opcode & 0x0FFFu);
    index = addr;
}

void Chip8::OP_Bnnn(){
    uint16_t addr = (opcode & 0x0FFFu);
    pc = reg[0] + addr;
}

void Chip8::OP_Cxkk(){
    uint8_t byte = (opcode & 0x00FFu);
    uint8_t reg_idx1 = (opcode & 0x0F00u) >> 8u;
    reg[reg_idx1] = (randByte(randGen) & 0xFFu) & byte;
}


void Chip8::OP_Dxyn(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    uint8_t height = (opcode & 0x000Fu);

    uint8_t xPos = reg[Vx]%VIDEO_WIDTH;
    uint8_t yPos = reg[Vy]%VIDEO_HEIGHT;

    reg[0xF] = 0;
    for(unsigned int row=0; row<height; row++){
        uint8_t spriteByte = memory[index + row];
        for(unsigned int col=0; col < 8; col++){
            uint8_t spritePixel = spriteByte & (0x80u >> col);
            uint32_t* screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos +col)];

            if(spritePixel){
                if(*screenPixel == 0xFFFFFFFF){
                    reg[0xF] = 1;
                } 

                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
}

void Chip8::OP_Ex9E(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t key = reg[Vx];

    if(keypad[key]){
        pc+=2;
    }
}

void Chip8::OP_ExA1(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t key = reg[Vx];

    if(!keypad[key]){
        pc+=2;
    }
}

void Chip8::OP_Fx07(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    reg[Vx] = delayTimer;
}

void Chip8::OP_Fx0A()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	if (keypad[0])
	{
		reg[Vx] = 0;
	}
	else if (keypad[1])
	{
		reg[Vx] = 1;
	}
	else if (keypad[2])
	{
		reg[Vx] = 2;
	}
	else if (keypad[3])
	{
		reg[Vx] = 3;
	}
	else if (keypad[4])
	{
		reg[Vx] = 4;
	}
	else if (keypad[5])
	{
		reg[Vx] = 5;
	}
	else if (keypad[6])
	{
		reg[Vx] = 6;
	}
	else if (keypad[7])
	{
		reg[Vx] = 7;
	}
	else if (keypad[8])
	{
		reg[Vx] = 8;
	}
	else if (keypad[9])
	{
		reg[Vx] = 9;
	}
	else if (keypad[10])
	{
		reg[Vx] = 10;
	}
	else if (keypad[11])
	{
		reg[Vx] = 11;
	}
	else if (keypad[12])
	{
		reg[Vx] = 12;
	}
	else if (keypad[13])
	{
		reg[Vx] = 13;
	}
	else if (keypad[14])
	{
		reg[Vx] = 14;
	}
	else if (keypad[15])
	{
		reg[Vx] = 15;
	}
	else
	{
		pc -= 2; // wait
	}
}

void Chip8::OP_Fx15()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	delayTimer = reg[Vx];
}

void Chip8::OP_Fx18()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	soundTimer = reg[Vx];
}

void Chip8::OP_Fx1E()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	index += reg[Vx];
}

void Chip8::OP_Fx29()
{
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t digit = reg[Vx];

    index = FONTSET_START_ADDR + (5 * digit);
}

void Chip8::OP_Fx33(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t value = reg[Vx];
    uint8_t I = index;
    
    memory[I+2] = value%10;
    value /= 10;
    memory[I+1] = value%10;
    value /= 10;
    memory[I] = value%10;
    value /= 10;
}

void Chip8::OP_Fx55(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    for(uint8_t i=0; i<Vx; ++i){
        memory[index + i] = reg[i];
    }
}

void Chip8::OP_Fx65(){
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    for(uint8_t i=0; i<Vx; ++i){
        reg[i] = memory[index+i];
    }
}

void Chip8::Table0(){
    uint8_t idx = (opcode & 0x000Fu);
    ((*this).*(table0[idx]))();
}

void Chip8::Table8(){
    uint8_t idx = (opcode & 0x000Fu);
    ((*this).*(table8[idx]))();
}

void Chip8::TableE(){
    uint8_t idx = (opcode & 0x000Fu);
    ((*this).*(tableE[idx]))();
}

void Chip8::TableF(){
    uint8_t idx = (opcode & 0x00FFu);
    ((*this).*(tableF[idx]))();
}

void Chip8::Cycle(){
    //Fetch
    opcode = (memory[pc] << 8u) | (memory[pc+1]);
    //std::cout << opcode << std::endl;
    pc+=2;
    //Decode and Execute
    ((*this).*(table[(opcode & 0xF000)>> 12u]))();

    if(delayTimer > 0) delayTimer--;
    if(soundTimer > 0) soundTimer--;
}