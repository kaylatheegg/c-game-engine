#include "engine.h"

int generateWorld() {
	//loadStructure("engine/data/structures/house.txt", 400, 400);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			int room = rand() % 5;
			switch (room) {
				case 0:
					loadStructure("engine/data/structures/Lwall.txt", i * 64 * 7, j * 64 * 7);
					break;

				case 1:
					loadStructure("engine/data/structures/horizontal_wall.txt", i * 64 * 7, j * 64 * 7);
					break;

				case 2:
					loadStructure("engine/data/structures/straight_wall.txt", i * 64 * 7, j * 64 * 7);
					break;

				case 3:
					loadStructure("engine/data/structures/4way.txt", i * 64 * 7, j * 64 * 7);
					break;

				case 4:
					loadStructure("engine/data/structures/Tway.txt", i * 64 * 7, j * 64 * 7);
					break;

				default:
					printf("what\n");
					break;
			}
		}
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
			y -= 64;
			x = xOrigin;
			continue;
		}
		if (structure[i] == ' ') {
			createObject("tile", (Rect){x, y, 64, 64}, 0, 0, 1, 0, getTexture("Floor"), 32);
			x+= 64;
			continue;
		}
		createEntity((object){.name = "wall",
    	   					 	   .rect = (Rect){x, y, 64, 64}, 
    	   						   .xOffset = 0,
    	   						   .yOffset = 0,
    	   						   .scale = 1.0,
    	   						   .angle = 0,
    	   						   .texture = getTexture("Wall"),
    	   						   .layer = 32}, COLLIDE_BOX,
		objHandler, NULL, 0,
		objCollisionHandler, &(body){.mass = 10, 
									 .velocity = VECCNT(0,0), 
									 .acceleration = VECCNT(0,0)});
		x += 64;
	}
	gfree(structure);
	return 0;
}
