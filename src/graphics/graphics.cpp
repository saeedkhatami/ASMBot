#include <graphics/graphics.h>
#include <game.h>

SDL_Window* ASMBots::Graphics::window = NULL;
SDL_Surface* ASMBots::Graphics::screenSurface = NULL;
SDL_Renderer* ASMBots::Graphics::renderer = NULL;

bool ASMBots::Graphics::initGraphics(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		fprintf(stderr, "Couldn't init sdl2: %s\n", SDL_GetError());
		return false;
	}

	SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);

	if (window == NULL || renderer == NULL) {
		fprintf(stderr, "could not create window or renderer: %s\n", SDL_GetError());
		return false;
	}

	screenSurface = SDL_GetWindowSurface(window);
	return true;
}

void ASMBots::Graphics::cleanupGraphics(){
	SDL_Quit();
}

void ASMBots::Graphics::draw(){
	SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

	for(int x = 0; x < 640; x += 20) {
		SDL_RenderDrawLine(renderer, x, 0, x, 480);
	}
	for(int y = 0; y < 480; y+= 20) {
		SDL_RenderDrawLine(renderer, 0, y, 640, y);
	}

	SDL_SetRenderDrawColor(renderer, 200, 50, 50, 200);
	SDL_Rect rect = {bot.pos.x - 5, bot.pos.y - 5, 10, 10};
	SDL_RenderFillRect(renderer, &rect);

	SDL_RenderPresent(renderer);
}