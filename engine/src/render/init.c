#include "../../includes/engine.h"

int initRender() {
	char error[256];
	
	logtofile("Initialising window", INF, "Render"); 
	window = SDL_CreateWindow("Game Engine",
							  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
							  SCREEN_WIDTH, SCREEN_HEIGHT,
							  SDL_WINDOW_SHOWN);
	if (!window) {
    	sprintf(error, "Initialising window failure!: %s", SDL_GetError());
    	logtofile(error, SVR, "Render");
    	crash();
	}

	logtofile("Initialising renderer", INF, "Render"); 
	renderer = SDL_CreateRenderer(window,
								  -1,
								  SDL_RENDERER_ACCELERATED);
	if (!renderer) {
    	sprintf(error, "Initialising renderer failure!: %s", SDL_GetError());
    	logtofile(error, SVR, "Render");
    	crash();
	}

	return 0;
}

void cleanRender() {
	logtofile("Destroying textures and dict.", INF, "Render");
	cleanTexture();

	logtofile("Destroying renderer", INF, "Render"); 
	SDL_DestroyRenderer(renderer);

	logtofile("Destroying window", INF, "Render"); 
	SDL_DestroyWindow(window);
}
