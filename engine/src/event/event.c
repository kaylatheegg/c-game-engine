#include "engine.h"


/**
 * @brief      Processes events
 */
void processEvents() {
	for (int i = 0; i < eventCount; i++) {
		SDL_Event event = events[i];

		if (event.type == SDL_QUIT) {
			running = 0;
		}

		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE: running = 0; break;
			}
		} else	{
			//logtofile("Unhandled SDL2 event sent to processEvents()!", INF, "Event");
		}
	}

}