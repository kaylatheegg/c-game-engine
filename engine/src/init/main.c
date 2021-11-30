#include "engine.h"

//NEVER EVER UNDO THE ID SYSTEM, GOD DAMNIT

/* TODO LIST

make a trello, this todo list is useless

rewrite vertex pool

hook all events into a multi array system to decouple it from sdl2's event system, which has
a real issue when you pump the events, leading to events being lost entirely.

multithread the entity handlers and the renderer (THIS WILL CAUSE ALL THE BUGS!!!)
*/


int main() {
	srand(time(0));
	int framerate = 60;
	running = 1;
	initLog();
	initSignalHandler();

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

 	loadTexture("engine/data/images/player.png", "Player");
	loadTexture("engine/data/images/sand.png", "Sand");
	loadTexture("engine/data/images/water.png", "Water");
	loadTexture("engine/data/images/burnt.png", "Burnt");
	loadTexture("engine/data/images/fire.png", "Fire");
	loadTexture("engine/data/images/grass.png", "Grass");
	loadTexture("engine/data/images/bullet.png", "Bullet");
	loadTexture("engine/data/images/enemyship.png", "Enemy");
	loadTexture("engine/data/images/health.png", "Healthbar");
	loadTexture("engine/data/images/healthback.png", "HealthbarBack");

	int currentTime = 0;
	int lastTime = 0;
	frameCount = 0;

	initWorld();

	logtofile("Initialisation Complete!", INF, "Runtime");
	
	Uint64 intStartFrame, intEndFrame = 0;
	Uint64 endFrame = 0;
	float intDt;
 	while (running) {
 		intStartFrame = SDL_GetPerformanceCounter();

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
    		sprintf(buffer, "FPS: %d, objCount: %d, render: %d", frameCount, objectCount, renderedObjects);
    		logtofile(buffer, INF, "Runtime");
   			lastTime = currentTime;
   			frameCount = 0;
   		}



   		intEndFrame = SDL_GetPerformanceCounter() - intStartFrame;
   		intDt = (double)(intEndFrame)/(double)SDL_GetPerformanceFrequency();
		//printf("%lf\n", dt);

   		SDL_Delay((double)intDt > (double)1/framerate ? 0 : (double)1000/framerate - (double)(intDt * 1000.));
   		
   		
   		//printf("startframe: %ld\n endframe: %ld\n diff in ms: %lf\n", startFrame, endFrame, (float)(endFrame-startFrame)/SDL_GetPerformanceFrequency());
   		//printf("dt: %lf\n", dt);
   		endFrame = SDL_GetPerformanceCounter() - intStartFrame;
   		dt = (double)(endFrame)/(double)SDL_GetPerformanceFrequency();
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