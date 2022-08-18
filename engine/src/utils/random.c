#include "engine.h"

//these are kinda hacky and fail most randomness tests but w/e i need some

int randRange(int range) {
	return rand() % range;
}

//generates a random number in a box, from -width/2 to +width/2 on x and
//									  from -height/2 to +height/2

vec randBox(int width, int height) {
	int x = -width/2 + randRange(width);
	int y = -height/2 + randRange(height);
	return VECCNT(x,y);
}

