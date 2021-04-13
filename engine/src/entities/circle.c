#include "engine.h"

typedef struct {
	float radius;
	vec direction;
} circle;

void circleHandler(entity** this) {
	circle* data = (circle*)(*this)->data;
	if ((*this)->data == NULL) {
		//logtofile("werhkgkjdgkhgdklgsdhljgd", SVR, "gdjgfsdjkgd");
		return;
	}
	vec intVec = vecAdd(VECCNT(ENTRECT(x), ENTRECT(y)), data->direction);
	


	entity** intEntity = AABBCollision(this);

	if (intVec.x > SCREEN_WIDTH - (ENTRECT(w) / 2) || intVec.x < 0 + (ENTRECT(w) / 2)) {
		data->direction.x *= -1;
	}

	if (intVec.y > SCREEN_HEIGHT - (ENTRECT(h) / 2)|| intVec.y < 0 + (ENTRECT(h) / 2)) {
		data->direction.y *= -1;
	}



	
	if (intEntity != NULL && intEntity != this) {
		if (strcmp((*intEntity)->object->name, "circle") == 0) {
			circle* intCircle = (circle*)(*intEntity)->data;
			vec normal = vecSub(VECCNT(ENTRECT(x), ENTRECT(y)), VECCNT((*intEntity)->object->rect.x, (*intEntity)->object->rect.y));
			if (vecLength(normal) < intCircle->radius + data->radius) {
				intCircle->direction = vecSub(intCircle->direction, vecScale(vecProj(intCircle->direction, normal), 2));	

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
	for (int i = 0; i < 10; i++) {
		createEntity("circle", (SDL_Rect){rand() % SCREEN_WIDTH - 32, rand() % SCREEN_HEIGHT - 32, 64, 64}, -32, -32, 1.0, 0, getTexture("Circle"), 1, circleHandler, &(circle){32., VECCNT(rand()%4 + 4,rand()%4 + 4)}, sizeof(circle));
	}
}