#include "engine.h"

/* TODO LIST

memory leaks from entity** system and needing to impl entity cleanup functions

strange rendering bug where screen flickers black in first second
- this could be something to do with the layer system, the flickering is reproduced when
  the window is moved with a vertex shader failure
- low priority tho

find a way to do colour escape sequences in windows

make sound subsystem openAL

need to add non-fragmented monochromatic textures

add typed vectors, because storing ints in a float format will lead to MANY issues

test collisions multiple times a frame until there are no more collisions

vertex pool fragmentation? this probably doesnt need to be done yet, its not a huge priority 
and things arent being created/destroyed often enough to warrant something like this

implement a generic resource loading function set

multithread the entity handlers and the renderer (THIS WILL CAUSE ALL THE BUGS!!!)

*/

/*notes
	the mouse y coordinate needs to be fixed to the correct coordinate space by using SCREEN_HEIGHT - y, as shown in the button UI code 
*/

/*
		y
		^
		|
		|
-x <----+----> x
		|
		|
		v
	   -y
*/

int engineStart();

#ifdef __WIN32__
int SDL_main(int argc, char* argv[]) {
	UNUSED(argc);
	UNUSED(argv);
	return engineStart();
}
#else
int main() {
	return engineStart();
}
#endif

int engineStart() {
	srand(time(0));
	int framerate = 60;
	running = 1;

	initLog();

	initSignalHandler();

	logtofile("Testing graphs!!", INF, "Runtime");

	Graph* graph = createGraph(NULL, 12);
	for (int i = 0; i < 256; i++) {
		GraphEdge edges[2];
		if ((i + 1) % 16 == 0) {
			edges[0] = (GraphEdge){.vertexIDs = {i, i+16},
					.weight = (float)rand() / (float)65535
					};
			addVertex(graph, edges, 1);
			continue;
		}
		edges[0] = (GraphEdge){.vertexIDs = {i, i+1},
					.weight = (float)rand() / (float)65535
					};
		edges[1] = (GraphEdge){.vertexIDs = {i, i+16},
					.weight = (float)rand() / (float)65535
					};
		
		addVertex(graph, edges, 2);
	}


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
	
 	logtofile("Initialising UI", INF, "Runtime");
 	initUI();

 	/*logtofile("Initialising audio", INF, "Runtime");
 	initAudio();*/
	
 	logtofile("Initialising Game", INF, "Runtime");
 	worldInit();


	logtofile("Initialisation Complete!", INF, "Runtime");
	
	Uint64 intStartFrame, intEndFrame = 0;
	Uint64 endFrame = 0;
	float intDt;
	physicsTime = 0;
	float totalDt = 0;
	stack* dtStack = createStack(sizeof(float), STACK_FILO);

 	while (running) {
 		intStartFrame = SDL_GetPerformanceCounter();

 		SDL_Event intEvent;
		for (eventCount = 0; SDL_PollEvent(&intEvent) && eventCount < 255; eventCount++) {
			events[eventCount] = intEvent;
		}

		if(eventCount > 255) {
			logtofile("Event count in array went above 255, consider looking into the bug!", WRN, "Runtime");
		}

		keyPresses = SDL_GetKeyboardState(NULL);

		drawGraph(graph, 16, 16);

		if (render() != 0) {
			crash();
		}

		processEvents();
		physicsTime += dt;
		runEntities();

   		intEndFrame = SDL_GetPerformanceCounter() - intStartFrame;
   		intDt = (double)(intEndFrame)/(double)SDL_GetPerformanceFrequency();
		//printf("%lf\n", dt);

   		SDL_Delay((double)intDt > (double)1/framerate ? 0 : (double)1000/framerate - (double)(intDt * 1000.));
   		//printf("startframe: %ld\n endframe: %ld\n diff in ms: %lf\n", startFrame, endFrame, (float)(endFrame-startFrame)/SDL_GetPerformanceFrequency());
   		//printf("dt: %lf\n", dt);
   		endFrame = SDL_GetPerformanceCounter() - intStartFrame;
   		dt = (double)(endFrame)/(double)SDL_GetPerformanceFrequency();
   		totalDt += dt;
   		//average fps code
   		float sigmaDt = 1.0;
		if (dtStack->array->arraySize > 4) {
			sigmaDt = 0.0;
			for (int i = 0; i < 5; i++) {
				sigmaDt += *(float*)getElement(dtStack->array, i);
			}
			popStack(dtStack);
			sigmaDt /= 5;
		}
		pushStack(dtStack, &dt);
		char buffer[200];
		sprintf(buffer, "%d\n", (int)(1/sigmaDt));
		drawText(buffer, 760, 800 - 24, 48, (RGBA){.rgba = 0xFF000000});

		sprintf(buffer, "Time: %dms\n", (int)floor(totalDt*1000));
		//drawText(buffer, 250, 762, 96, (RGBA){.rgba = 0xFF000000});
   		frameCount++;
 	}

 	logtofile("Destroying entities", INF, "Runtime");
 	cleanEntities();
 	logtofile("Destroying objects", INF, "Runtime");
 	cleanObjects();

 	logtofile("Destroying rendering", INF, "Runtime");
 	cleanRender();

 	logtofile("Destroying SDL2", INF, "Runtime");
 	SDL_Quit();

	logtofile("Closing Engine! So long, and goodnight!", INF, "Runtime");
	return 0;
}
