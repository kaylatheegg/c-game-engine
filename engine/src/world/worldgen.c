#include "engine.h"

typedef struct {
	dynArray* walls;
	int height;
	int width;
} wallStruct;

wallStruct walls = {NULL, 0, 0};

int generateWorld() {
	loadStructure("engine/data/structures/house.txt", 0, 0);
	loadStructure("engine/data/structures/house.txt", 0, 0);
	/*for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			int room = rand() % 5;
			switch (room) {
				case 0:
					loadStructure("engine/data/structures/Lwall.txt", i * 7, j * 7);
					break;

				case 1:
					loadStructure("engine/data/structures/horizontal_wall.txt", i * 7, j * 7);
					break;

				case 2:
					loadStructure("engine/data/structures/straight_wall.txt", i * 7, j * 7);
					break;

				case 3:
					loadStructure("engine/data/structures/4way.txt", i * 7, j * 7);
					break;

				case 4:
					loadStructure("engine/data/structures/Tway.txt", i * 7, j * 7);
					break;

				default:
					printf("what\n");
					break;
			}
		}
	}*/      
	for (int i = 0; i < walls.height; i++) {
		for (int j = 0; j < 0; j++) {
			entity** intEntity = getEntityByID(*(int*)getElement(*(dynArray**)getElement(walls.walls, j), i));
			if (intEntity == NULL) { continue; }
			char* txFun[3] = {"cWall", "vWall", "hWall"};
			(*intEntity)->object->texture = getTexture(txFun[rand() % 3]);
			printf("| %s\n ", (*intEntity)->object->name);
		}
		printf("\n");
	}
	return 0;
}

void objHandler(entity** this) {
	UNUSED(this);
	return;
}

void objCollisionHandler(entity** this, entity** collision, float distance) {
	UNUSED(this);
	UNUSED(collision);
	UNUSED(distance);
}




void createWall(int x, int y) {
	//bug here! if you attempt to create a wall ontop of another wall, it may cause a crash. sanitise this!
	UNUSED(x);
	UNUSED(y);
	if (walls.walls == NULL) {
		walls.walls = createDynArray(sizeof(dynArray*));
		dynArray* internal = createDynArray(sizeof(int));
		insertElement(walls.walls, &internal, 0);
	}

	if (walls.height < y) {
		for (int i = walls.height; i < y; i++) {
			dynArray* internal = createDynArray(sizeof(int));
			insertElement(walls.walls, &internal, i);
		}
		walls.height = y;
	}

	if (getElement(walls.walls, y) == NULL) {
		printf("aaah you mad\n");
		crash();
	}

	if (x < walls.width) {
		if (*(int*)getElement(*(dynArray**)getElement(walls.walls, y), x) != 0) { //this still errors, the objects arent detected correctly.
			logtofile("Tried to place two walls ontop of eachother!", ERR, "World");
			return;

			//not working because memory is malloced, not calloced. 
			//might not be the issue tho?
		}
	}

	float angle = 0;
	int_Texture* tex = getTexture("cWall");

	int id = createEntity((object){.name = "wall",
		 	   .rect = (Rect){x*64, y*64, 64, 64}, 
			   .xOffset = 0,
			   .yOffset = 0,
			   .scale = 1.0,
			   .angle = angle,
			   .texture = tex,
			   .layer = 32}, COLLIDE_BOX,
				objHandler, NULL, 0,
				objCollisionHandler, &(body){.mass = 10, 
											 .velocity = VECCNT(0,0), 
											 .acceleration = VECCNT(0,0)});
	UNUSED(id);
	//printf("x:%d y:%d\n", x, y);
	dynArray* yIndexArray = *(dynArray**)getElement(walls.walls, 0);
	if (yIndexArray == NULL) {
		printf("shit the bed\n");
	}
	insertElement(yIndexArray, &id, x); // this may not be working correctly

}

int loadStructure(char* filename, int x, int y) {
	if (filename == NULL) {
		logtofile("Tried to load invalid room!", ERR, "World");
		return -1;
	}

	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		logtofile("Could not open file:", ERR, "World");
		printf("%s\n", filename);
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	rewind(fp);
	char* structure = gmalloc(sizeof(*structure) * size);
	fread(structure, size, 1, fp);
	fclose(fp);
	int yOrigin = y;
	UNUSED(yOrigin);
	int xOrigin = x;
	for (size_t i = 0; i < size; i++) {
		if (structure[i] == '\n') {
			y += 1;
			x = xOrigin;
			continue;
		}
		if (structure[i] == ' ') {
			createObject("tile", (Rect){x * 64, y * 64, 64, 64}, 0, 0, 1, 0, getTexture("Floor"), 32);
			x += 1;
			continue;
		}
		createWall(x++, y);
	}
	gfree(structure);
	return 0;
}