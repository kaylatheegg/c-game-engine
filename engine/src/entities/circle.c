#include "engine.h"

typedef struct {
	float radius;
	vec direction;
} circle;

void circleHandler(entity** this) {
	circle* data = (*this)->data;
	if ((*this)->data == NULL) {
		//logtofile("werhkgkjdgkhgdklgsdhljgd", SVR, "gdjgfsdjkgd");
		return;
	}
	
	vec intVec = vecAdd(VECCNT(ENTRECT(x), ENTRECT(y)), data->direction);
	


	ENTRECT(w) = data->radius * 2;
	ENTRECT(h) = data->radius * 2;
	(*this)->object->xOffset = data->radius * -1;
	(*this)->object->yOffset = data->radius * -1;

	entity** intEntity = AABBCollision(this);

	if (intVec.x > SCREEN_WIDTH - (ENTRECT(w) / 2.) || intVec.x < 0 + (ENTRECT(w) / 2)) {
		data->direction = vecSub(data->direction, vecScale(vecProj(data->direction, VECCNT(1, 0)), 2));

	}

	if (intVec.y > SCREEN_HEIGHT - (ENTRECT(h) / 2)|| intVec.y < 0 + (ENTRECT(h) / 2)) {
		data->direction = vecSub(data->direction, vecScale(vecProj(data->direction, VECCNT(0, 1)), 2));
	}

	
	if (intEntity != NULL && intEntity != this) {
		if (strcmp((*intEntity)->object->name, "circle") == 0) {
			circle* intCircle = (*intEntity)->data;
			vec normal = vecSub(VECCNT(ENTRECT(x), ENTRECT(y)), VECCNT((*intEntity)->object->rect.x, (*intEntity)->object->rect.y));
			if (vecLength(normal) < intCircle->radius + data->radius) {
				intCircle->direction = vecSub(intCircle->direction, vecScale(vecProj(intCircle->direction, vecScale(normal, -1)), 2));	

				(*intEntity)->object->rect.x += intCircle->direction.x;
				(*intEntity)->object->rect.y += intCircle->direction.y;

				data->direction = vecSub(data->direction, vecScale(vecProj(data->direction, normal), 2));	
				ENTRECT(x) += data->direction.x;
				ENTRECT(y) += data->direction.y;
			}
		}
	}
	ENTRECT(x) += data->direction.x;
	ENTRECT(y) += data->direction.y;

}

void initCircles() {
	for (int i = 0; i < 15; i++) {
		createEntity("circle", (SDL_Rect){i*33 + 32 , rand() % SCREEN_HEIGHT, 64, 64}, -32, -32, 1.0, 0, getTexture("Circle"), 1, circleHandler, &(circle){rand() % 16 + 16., VECCNT(rand()%4 + 4,rand()%4 + 4)}, sizeof(circle));

	}	

}