#include "../../includes/engine.h"

int render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	//start of object rendering
	dictionary current = objects;
	if (current == NULL) {
		logtofile("hey what the fuck, the objects dict is GONE.", SVR, "Render");
		crash();
	}

	current = current->next;

	while (current != NULL) {
		object* intObject = current->value;
		if (intObject == NULL) {
			logtofile("object dict value is null, this do be bad doe fr doe", SVR, "Render");
			crash();
		}
		SDL_Rect* rect = intObject->rect;

		//implement VI system
		//implement render bool

		if ((rect->x <= (SCREEN_WIDTH + rect->w) && rect->x >= (0 - rect->w)) && //ensure x is in the screen
			(rect->y <= (SCREEN_HEIGHT + rect->h) && rect->y >= (0 - rect->h))) //ensure y is in the screen
		{
				if (SDL_RenderCopyEx(renderer, intObject->texture, NULL, rect, intObject->angle, NULL, SDL_FLIP_NONE) != 0) {
					if (SDL_RenderCopyEx(renderer, getTexture("DEFAULT"), NULL, rect, intObject->angle, NULL, SDL_FLIP_NONE) != 0) {
						char error[512];
						sprintf(error, "SDL texture rendering failure, error: %.256s", SDL_GetError());
						logtofile(error, SVR, "Render");
						logtofile("failed to render with default texture, something has gone TERRIBLY WRONG!!", SVR, "Render");
						crash();
					}
				}
		}

		current = current->next;
	}


	//frameCount++;
	SDL_RenderPresent(renderer);
	return 0;
}