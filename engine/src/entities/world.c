#include "engine.h"

void cameraHandler(entity** this) {
	int speed = 5;
	if (keyPresses[SDL_SCANCODE_W]) {
		ENTRECT(y) -= speed;
		viewport.y -= speed;
	} if (keyPresses[SDL_SCANCODE_A]) {
		ENTRECT(x) -= speed;
		viewport.x += speed;
	} if (keyPresses[SDL_SCANCODE_S]) {
		ENTRECT(y) += speed;
		viewport.y += speed;
	} if (keyPresses[SDL_SCANCODE_D]) {
		ENTRECT(x) += speed;
		viewport.x -= speed;
	}


	if (rand() % 50 == 1) {
		//world[rand() % 50][rand() % 50].type = FIRE;
	}
	//glViewport(viewport.x, viewport.y, SCREEN_WIDTH, SCREEN_HEIGHT);
}

/*void mouseHandler(entity** this) {
	int x,y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);
	ENTRECT(x) = x - viewport.x;// - x % 32;
	ENTRECT(y) = -y - viewport.y;// - y % 32;
	//(*this)->object->xOffset = -viewport.x;
	//(*this)->object->yOffset = -viewport.y;



	SDL_PumpEvents();
	if ((buttons & SDL_BUTTON_LMASK) != 0) {
		entity** intEntity = AABBCollision(this);
		if (intEntity != NULL && intEntity != this) {
			deleteEntity(intEntity);
			//printf("%d\n", intObject->id);
		}
	}
}*/

int tileTest(int x, int y, int type) {
	int direction[4] = {1,1,1,1};
	if (x == 0) {direction[0] = 0;}
	if (x == worldWidth - 1) {direction[1] = 0;}
	if (y == 0) {direction[2] = 0;}
	if (y == worldHeight - 1) {direction[3] = 0;}

	if (direction[0] == 1) {
		if (world[x-1][y].type == type) {
			return 1;
		}
	}

	if (direction[1] == 1) {
		if (world[x+1][y].type == type) {
			return 1;
		}
	}

	if (direction[2] == 1) {
		if (world[x][y-1].type == type) {
 			return 1;
		}
	}

	if (direction[3] == 1) {
		if (world[x][y+1].type == type) {
			return 1;
		}
	}
	return 0;
}

void tileHandler(entity** this) {
	int updatedTile = 0;
	tileData* data = (*this)->data;
	
	/*data->x = world[data->x][data->y].x;
	data->y = world[data->x][data->y].y;
	data->ID = world[data->x][data->y].ID;
	data->type = world[data->x][data->y].type;
	data->tileUpdate = world[data->x][data->y].tileUpdate;*/

	if (data->type == DIRT) {
		if ((*this)->object->texture != getTexture("Sand")) {
			(*this)->object->texture = getTexture("Sand"); 
			updatedTile = 1;
		}
		
		if ((rand() % 200000000) == 1) {
			data->type = GRASS;
			world[data->x][data->y].type = GRASS;
			updatedTile = 1;
		}


		if ((rand() % 1200) != 1) {
			goto afterTest;
		}

		if (tileTest(data->x, data->y, GRASS) == 1) {
			data->type = GRASS;
			world[data->x][data->y].type = GRASS;
			updatedTile = 1;
		}
	}

	if (data->type == GRASS) {
		if ((*this)->object->texture != getTexture("Grass")) {
			(*this)->object->texture = getTexture("Grass"); 
			updatedTile = 1;
		}
		
	

		if ((rand() % 45) != 1) {
			goto afterTest;
		}

		if (tileTest(data->x, data->y, FIRE) == 1) {
			data->type = FIRE;
			world[data->x][data->y].type = FIRE;
			updatedTile = 1;
		}
	}

	if (data->type == FIRE) {
		if ((*this)->object->texture != getTexture("Fire")) {
			(*this)->object->texture = getTexture("Fire"); 
			updatedTile = 1;
		}

		if ((rand() % 60) == 1) {
			
			data->type = BURNT;
			world[data->x][data->y].type = BURNT;
			updatedTile = 1;
			goto afterTest;
		}
	}

	if (data->type == BURNT) {
		if ((*this)->object->texture != getTexture("Burnt")) {
			(*this)->object->texture = getTexture("Burnt"); 
			updatedTile = 1;
		}

		if ((rand() % 60) == 1 && tileTest(data->x, data->y, GRASS) == 1) {
			data->type = DIRT;
			world[data->x][data->y].type = DIRT;
			updatedTile = 1;
		}
	}

	afterTest:
	if (updatedTile == 1) {
		updateObject((*this)->object);
	}
	return;
}



void addTile(int x, int y, int type) {
	createEntity("tile", (Rect){x * 48,y * 48, 48, 48}, 0, 0, 1.0, 0, getTexture("Sand"), COLLIDE_NONE, tileHandler, &(tileData){x,y,type,1}, sizeof(tileData));
	if (worldHeight < y) {
		world = grealloc(world, sizeof(*world) * (y + 1));
		for (int i = worldHeight; i < (y+1); i++) {
			world[i] = gmalloc(sizeof(**world) * (worldWidth + 1));
			for (int j = 0; j < worldWidth; j++) {
				world[i][j] = (tileData){0,0,0,0,0};
			}
		}
		worldHeight = y;
	}
	if (worldWidth < x) {
		for (int i = 0; i < worldHeight + 1; i++) {
			if (world[i] == NULL) {
				world[i] = gmalloc(sizeof(**world));
			}
			world[i] = grealloc(world[i], sizeof(**world) * (x + 1));
		}
		worldWidth = x;
	}
	world[y][x] = (tileData){x,y,type,1, 0};
}

int initWorld() {
	worldWidth = 1;
	worldHeight = 1;
	world = gmalloc(sizeof(*world));
	*world = gmalloc(sizeof(**world));
	/*for (int i = 0; i < WORLDWIDTH; i++) {
		for (int j = 0; j < WORLDHEIGHT; j++) {
			world[i][j].ID = createEntity("tile", (Rect){i*48,j*48, 48, 48}, 0, 0, 1.0, 0, getTexture("Sand"), COLLIDE_NONE, tileHandler, &(tileData){i,j, GRASS,0}, sizeof(tileData));
			world[i][j].x = i;
			world[i][j].y = j;
			world[i][j].type = GRASS;
			world[i][j].tileUpdate = 0;
		}
	}*/



	for (int i = 0; i < 16; i++) {
		//printf("%d\n", i);
		int width = 4+rand()%16;
		int height = 4+rand()%16;
		int x = rand() % 100;
		int y = rand() % 100;
		for (int j = 0; j < width; j++) {
			addTile(x + j, y, GRASS);
			addTile(x + j, y + height - 1, GRASS);	
		}
		for (int j = 1; j < height - 1; j++) {
			addTile(x, y + j, GRASS);
			addTile(x + width - 1, y + j, GRASS);
		}
	}
	
	//world[5][5].type = GRASS;

	//world[8][8].type = FIRE;

	//createEntity("camera", (SDL_Rect){300, 600, 0, 0}, 0, 0, 1.0, 0, getTexture("DEFAULT"), 0, cameraHandler, NULL, 0);
	initPlayer();
	//initAnimals();
	//createEntity("mouse", (SDL_Rect){0, 0, 32, 32}, 0, 0, 1.0, 0, getTexture("DEFAULT"), 0, mouseHandler, NULL, 0);
	return 0;
}