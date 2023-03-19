#include "engine.h"

/*
	for future kayla:
	the chunk system works by assigning each 16x16 set of tiles in the world a struct
	this struct is stored in the chunks dictionary with a key of X-Y, where X and Y are the
	grid locked positions of each of these chunks, with origins at worldspace 0,0.


*/

void updateWalls();
void createWall(int x, int y);

int generateWorld() {
	
	//createChunk(1,0);
	//createChunk(0,4);
	//createChunk(5,3);
	//createChunk(1,2);
	//loadStructure("engine/data/structures/test.txt", 0, 0);
	//loadStructure("engine/data/structures/house.txt", 0, 0);
	for (int i = 0; i < 0; i++) {
		for (int j = 0; j < 16; j++) {
			if (rand() % 2 == 0) {
				createWall(i,j);
			}
		}
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			loadStructure("engine/data/structures/4way.txt", i * 5, j * 5);
			continue;
			int room = rand() % 5;
			switch (room) {
				case 0:
					loadStructure("engine/data/structures/Lwall.txt", i * 5, j * 5);
					break;

				case 1:
					loadStructure("engine/data/structures/horizontal_wall.txt", i * 5, j * 5);
					break;

				case 2:
					loadStructure("engine/data/structures/straight_wall.txt", i * 5, j * 5);
					break;

				case 3:
					loadStructure("engine/data/structures/4way.txt", i * 5, j * 5);
					break;

				case 4:
					loadStructure("engine/data/structures/Tway.txt", i * 5, j * 5);
					break;

				default:
					printf("what\n");
					break;
			}
		}
	}     
	updateWalls();
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


void updateWalls() {
	if (chunks == NULL) {
		return;
	}
	for (size_t i = 0; i < chunks->key->arraySize; i++) {
		chunk* intChunk = *(chunk**)getElement(chunks->value, i);
		if (intChunk == NULL) {
			logtofile("Found a NULL chunk while updating walls, crashing!", ERR, "World");
			crash();
		}
		int** ids = intChunk->ids;
		int x = intChunk->X * 16;
		int y = intChunk->Y * 16;
		//printf("printing chunk X:%d Y:%d\n", intChunk->X, intChunk->Y);
		for (int j = 0; j < 16; j++) {
			for (int k = 0; k < 16; k++) {
				//printf("| %02d ", ids[k][15-j]);
			}
			//printf("|\n");
		}
		UNUSED(ids);
		for (int j = 0; j < 16; j++, y++) {
			x = intChunk->X * 16;
			for (int k = 0; k < 16; k++, x++) {
				//int ax = abs(x);
				//int ay = abs(y);
				if (ids[k][j] == 0) {
					//printf("0 at x:%d y:%d\n", x, y);
					continue;
				}
				//determine tiles around this one
				entity** intEntity = getEntityByID(ids[k][j]);
				if (intEntity == NULL) {
					crash();
					continue;
				}
				int tiles = 0;
				if (checkTile(x, y + 1) == 1) { tiles ^= 0x1; }
				if (checkTile(x + 1, y) == 1) { tiles ^= 0x2; }
				if (checkTile(x, y - 1) == 1) { tiles ^= 0x4; }
				if (checkTile(x - 1, y) == 1) { tiles ^= 0x8; }
				//printf("x: %d y: %d tileType: %d\n", x, y, tiles);
				char* txName = "DEFAULT";
				float angle = 0;
				switch (tiles) {
					case 0:	 //pillar    tx: pillar
						txName = "pWall";
						break;

					case 1:  //edge   N  tx: edge
						txName = "eWall";
						angle = 180.f;
						break;

					case 2:  //edge   E  tx: edge
						txName = "eWall";
						angle = 90.f;
						break;

					case 3:  //corner NE tx: corner *
						txName = "cWall";
						angle = 90.f;				
						break;

					case 4:  //edge   S  tx: edge
						txName = "eWall";
						break;

					case 5:  //line   V  tx: wall
						txName = "lWall";
						angle = 90.f;
						break;

					case 6:  //corner SE tx: corner *
						txName = "cWall";
						break;

					case 7:  //t-way  W  tx: t-way +
						txName = "tWall";
						angle = 90.f;
						break;

					case 8:  //edge   W  tx: edge
						txName = "eWall";
						angle = 270.f;
						break;

					case 9:  //corner NW tx: corner *
						txName = "cWall";
						angle = 180.f;

						break;

					case 10: //line   H  tx: wall
						txName = "lWall";
						break;

					case 11: //t-way  S  tx: t-way +
						txName = "tWall";
						angle = 180.f;	
						break;
					case 12: //corner SW tx: corner *
						txName = "cWall";
						angle = 270.f;
						break;
					case 13: //t-way  E  tx: t-way +
						txName = "tWall";
						angle = 270.f;
						break;
					case 14: //t-way  N  tx: t-way +
						txName = "tWall";
						break;
					case 15: //cross     tx: cross
						txName = "crWall";
						break;
					default:
						logtofile("something is incredibly broken, the dynamic walls are BROKE", ERR, "World");
						printf("tile Type: %d\n", tiles);
						crash();
						break;

				}
				(*intEntity)->object->texture = getTexture(txName);
				(*intEntity)->object->angle = angle;
				updateObject((*intEntity)->object);
			}
		}
	}
}

void createWall(int x, int y) {
	UNUSED(x);
	UNUSED(y);
	if (checkTile(x,y) == 1) {
		logtofile("Attempted to add a tile ontop of another one!", ERR, "World");
		printf("tiles: x:%d, y:%d\n", x, y);
		return;
	}

	float angle = 12;
	int_Texture* tex = getTexture("DEFAULT");

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
	if(addTile(x, y, id) == 1) {
		crash();
	}	
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
			y -= 1;
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

int addTile(int x, int y, int id) {
	int X = x;
	int Y = y;
	X &= 0xFFFFFFF0;
	X /= 0x10;

	Y &= 0xFFFFFFF0;
	Y /= 0x10;

	//printf("x:%d X:%d\n", x, X);
	if (checkTile(x,y) == -1) {
		//printf("we making chunk: %d-%d\n", X, Y);
		createChunk(X,Y);
	}

	if (checkTile(x,y) == 0) {
		chunk* intChunk = findChunk(x,y);
		intChunk->ids[x & 0xF][y & 0xF] = id;
		//printf("we making tile x: %d y: %d in chunk: %d-%d\n", x, y, intChunk->X, intChunk->Y);
		return 0;
	} else {
		return 1;
	}
}



int checkTile(int x, int y) {
	chunk* intChunk = findChunk(x, y);
	if (intChunk == NULL) {
		//printf("lmao we testing\n");
		return -1;
	}
	//printf("finding chunk at x:%d y:%d yielded: 0x%08lx\n", x, y, (size_t)intChunk);
	//printf("chunk is at X:%d, Y:%d\n", intChunk->X, intChunk->Y);

	if (intChunk->ids == NULL) {
		logtofile("Failed to find tile ids in a valid chunk, crashing!", SVR, "World");
		crash();
	}

	//printf("x:%d y:%d\n", x & 0xF, y & 0xF);
	if (intChunk->ids[x & 0xF][y & 0xF] != 0) {
		return 1;
	} else {
		return 0;
	}
}


chunk* findChunk(int x, int y) {
	if (chunks == NULL) {
		return NULL;
	}	

	//truncate x and y
	x &= 0xFFFFFFF0;
	x /= 0x10;

	y &= 0xFFFFFFF0;
	y /= 0x10;

	char name[9];
	sprintf(name, "%d-%d", x, y);
	//printf("findChunk name: %s\n", name);
	size_t index = findKey(chunks, name);
	if (index == NOVALUE) {
		return NULL;
	}
	chunk* foundChunk = *(chunk**)getElement(chunks->value, index);
	//printf("chunk index: %ld\nchunk pointer at 0x%08lx\n", index, (size_t)foundChunk);
	return foundChunk;
}

int createChunk(int X, int Y) {
	if (chunks == NULL) {
		chunks = createDictionary();
	}

	chunk* val = findChunk(X*16, Y*16);
	if (val != NULL) {
		return -1;
	}

	int** ids = gzalloc(16*sizeof(int*));
	for (int i = 0; i < 16; i++) {
		ids[i] = gzalloc(16 * sizeof(int));
	}

	//really need to add a gcalloc
	chunk* newChunk = gzalloc(sizeof(chunk));
	*newChunk = (chunk){.X = X, .Y = Y, .ids = ids};

	//truncate chunk length to 4 digits. this is not implemented yet BUT

	char name[9];
	sprintf(name, "%d-%d", X, Y);
	//printf("createChunk name: %s\n", name);
	addToDictionary(chunks, name, newChunk);
	return 0;
}




/*
chunk layout for storing tiles
relative around 0,0, the chunk can be stored in a dictionary with a name of X-Y
     |
     |+++++
	 |+++++
-----+-----
     |
     |
     |
determining the chunk can be used with a truncation with
&= 0xFFFFFFFF ^ 0b1111 (top 4 bits removed bitmask)
/= 0x10 (chunk size)
to determine a chunk id.
this allows a maximum backrooms world size of 2^32 chunks, and 2^36 blocks
during tile insertion, an x,y tile can be converted to X-Y coordinates to look up in the
dictionary. if it does not exist, then it can be created and added into the dictionary.

chunk origin is at the bottom left, with increasing X and Y according to the world
coordinate space.






*/