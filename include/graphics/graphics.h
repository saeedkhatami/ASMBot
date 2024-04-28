#ifndef ASMBOTS_GRAPHICS_H
#define ASMBOTS_GRAPHICS_H

#include <SDL2/SDL.h>

namespace ASMBots::Graphics {
	extern SDL_Window* window;
	extern SDL_Surface* screenSurface;
	extern SDL_Renderer* renderer;

	bool initGraphics();
	void cleanupGraphics();
	void draw();
};

#endif //ASMBOTS_GRAPHICS_H