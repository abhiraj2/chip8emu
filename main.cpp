#include "chip8.h"
#include "platform.h"

#include<chrono>
#include<iostream>
#include<string>


int main(int argc, char* argv[]) {
	std::cout << "Hello" << std::endl;
	if (argc != 4) {
		std::cerr << "Usage: " << argv[0] << "<Scale> <Delay> <ROM>\n";
		std::exit(EXIT_FAILURE);
	}

	int videoScale = std::stoi(argv[1]);
	int delay = std::stoi(argv[2]);
	char const* romFileName = argv[3];

	Platform platform("Chip-8 Emulator", VIDEO_WIDTH * videoScale, VIDEO_HEIGHT * videoScale, VIDEO_WIDTH, VIDEO_HEIGHT);

	Chip8 chip8;
	chip8.LoadROM(romFileName);
	
	int videoPitch = sizeof(chip8.video[0]) * VIDEO_WIDTH;
	auto lastCycleTime = std::chrono::high_resolution_clock::now();
	bool quit = false;

	while (!quit) {
		
		quit = platform.ProcessInput(chip8.keypad);

		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();
		//std::cout << delay << "\t" << dt << std::endl;
		if(dt > delay ){
			//std::cout << "Hello" << std::endl;
			lastCycleTime = currentTime;
			chip8.Cycle();
			platform.Update(chip8.video, videoPitch);
		}
	}

	return 0;
}