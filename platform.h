#ifndef PLATFORM_H
#define PLATFORM_H

#include<SDL.h>

class Platform
{
public:
	Platform(char const* title, int windowWidth, int windowHeight, int texttureWidth, int textureHeight);
	~Platform();
	void Update(void const* buffer, int pitch);
	bool ProcessInput(uint8_t* keys);

private:
	SDL_Window* window{};
	SDL_Renderer* renderer{};
	SDL_Texture* texture{};
};

#endif // !PLATFORM_H
