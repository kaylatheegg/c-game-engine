#include "../../includes/engine.h"

/* TODO LIST

add a wrapper around the stdlib memory handling functions to detect freeing in stuff like .rodata, and safely handle it without causing a crash


hook all events into a multi array system to decouple it from sdl2's event system, which has
a real issue when you pump the events, leading to events being lost entirely.

multithread the entity handlers and the renderer (THIS WILL CAUSE ALL THE BUGS!!!)

lowest priority: occasionally go through ubsan w/ a fine tooth comb and get rid of memory leaks you fuck


*/


int main() {
	int framerate = 60;
	running = 1;
	initLog();
	logtofile("Starting Engine!", INF, "Runtime");

	logtofile("Initialising SDL2", INF, "Runtime");
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    	char error[512];
    	sprintf(error, "Initialising SDL2 failure!: %.256s\n", SDL_GetError());
    	logtofile(error, SVR, "Runtime");
    	crash();
 	}


 	logtofile("Initialising rendering", INF, "Runtime");
 	initRender();

 	logtofile("Initialising objects", INF, "Runtime");
 	initObjects();

 	logtofile("Initialising entities", INF, "Runtime");
 	initEntities();

	loadTexture("engine/data/images/brick.png", "Brick");
	loadTexture("engine/data/images/player.png", "Player");
	loadTexture("engine/data/images/sand.png", "Sand");
	loadTexture("engine/data/images/water.png", "Water");
	loadTexture("engine/data/images/star.png", "Star");
	loadTexture("engine/data/images/playership.png", "Ship");
	loadTexture("engine/data/images/bullet.png", "Bullet");
	loadTexture("engine/data/images/enemyship.png", "Enemy");

 	//printTextures();
	//createObject("Test", 0, 0, 32, 32, 0, 0, 1, 0, getTexture("DEFAULT"));

	starInit();
	playerInit();

	char buffer[19];
	itoa(0, buffer);



	//printDictionary(objects);

	int currentTime = 0;
	int lastTime = 0;
	frameCount = 0;

	Uint64 startFrame, endFrame;
 	while (running) {
 	

 		startFrame = SDL_GetPerformanceCounter();

 		SDL_Event intEvent;
		for (eventCount = 0; SDL_PollEvent(&intEvent) && eventCount < 255; eventCount++) {
			events[eventCount] = intEvent;
		}

		if(eventCount > 255) {
			logtofile("Event count in queue went above 255, consider looking into the bug!", WRN, "Runtime");
		}

		keyPresses = SDL_GetKeyboardState(NULL);

		processEvents();

		runEntities();

		if (render() != 0) {
			crash();
		} 



		currentTime = SDL_GetTicks();
  		if (currentTime > lastTime + 1000) {
  			char buffer[256];
    		sprintf(buffer, "FPS: %d, objCount: %d", frameCount, objectCount);
    		logtofile(buffer, INF, "Runtime");
   			lastTime = currentTime;
   			frameCount = 0;
   		}



   		SDL_Delay(1000/framerate);
   		endFrame = SDL_GetPerformanceCounter();
   		dt = (float)(endFrame - startFrame) / (float)SDL_GetPerformanceFrequency();
   		//printf("dt: %lf\n", dt);
   		frameCount++;
 	}

 	logtofile("Destroying objects", INF, "Runtime");
 	cleanObjects();

 	logtofile("Destroying rendering", INF, "Runtime");
 	cleanRender();

 	logtofile("Destroying SDL2", INF, "Runtime");
 	SDL_Quit();

	logtofile("Closing Engine! So long, and goodnight!", INF, "Runtime");
	return 0;
}