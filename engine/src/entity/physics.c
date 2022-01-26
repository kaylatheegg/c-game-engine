#include "engine.h"

// positive y = up
// positive x = right

void processPhysics() {
	float physicsTimeStep = dt;
	for (size_t i = 0; i < entities->key->arraySize; i++) {
		entity** intEntity = (*(entity***)getElement(entities->value, i));
		//v = u + at

		if (vecLength((*intEntity)->body->acceleration) > physicsData.maxAcceleration) {
			(*intEntity)->body->acceleration = vecScale(vecNorm((*intEntity)->body->acceleration), 2000);
		}

		vec velocity = vecAdd(vecScale((*intEntity)->body->acceleration, physicsTimeStep), (*intEntity)->body->velocity);
		if (vecLength(velocity) > physicsData.maxVelocity) {
			velocity = vecScale(vecNorm(velocity), 2000);
		}

		(*intEntity)->body->velocity = velocity;
		(*intEntity)->object->rect.x += velocity.x;
		(*intEntity)->object->rect.y += velocity.y;
	}
}

void initPhysics() {
	physicsData = (universe){
		.maxForce = 2000,
		.maxVelocity = 2000,
		.maxAcceleration = 100
	};
}

