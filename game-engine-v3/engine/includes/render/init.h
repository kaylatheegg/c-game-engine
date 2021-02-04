#pragma once
#define RENDER_INIT_H

SDL_Window *window;
SDL_Renderer *renderer;

int initRender();

void cleanRender();