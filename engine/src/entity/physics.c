#include "engine.h"

// positive y = up
// positive x = right




void processPhysics() {
	
	if (entities->key->arraySize == 0) {
		return;
	}
	float KE = 0;
	//printf("%f\n", physicsTime);
	for (int step = 0; step < 1; step++) {
	float physicsTimeStep = dt/1.;
	testCollision();
	//temporary bound checking on objects
	for (size_t i = 0; i < collideArray->arraySize; i++) {

		collidePair* intPair = getElement(collideArray, i);
		entity* entityA = intPair->a;
		
		entity* entityB = intPair->b;

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

		if (entityA->collide == COLLIDE_BOX && entityB->collide == COLLIDE_BOX) {
			//determine distance between two boxes
			Rect eA = entityA->object->rect;
			Rect eB = entityB->object->rect;
			float dx = 0.0;
			float dy = 0.0;
			if (eA.x < eB.x) {
				dx = eB.x - (eA.x + eA.w);
			} else if (eA.x > eB.x) {
				dx = eA.x - (eB.x + eB.w);
			}

			if (eA.y < eB.y) {
				dy = eB.y - (eA.y + eA.h);
			} else if (eA.y > eB.y) {
				dy = eA.y - (eB.y + eB.h);
			}

			float xTime = entityA->body->velocity.x != 0 ? fabs(dx / entityA->body->velocity.x) : 0;
			float yTime = entityA->body->velocity.y != 0 ? fabs(dy / entityA->body->velocity.y) : 0;
	
			float leastTime = 0.0;
			float intendedDX = 0.0;
			float intendedDY = 0.0;
			if (entityA->body->velocity.x != 0 && entityA->body->velocity.y == 0) {
				leastTime = xTime;
				intendedDX = leastTime*entityA->body->velocity.x;
			} else if (entityA->body->velocity.x == 0 && entityA->body->velocity.y != 0) {
				leastTime = yTime;
				intendedDY = leastTime*entityA->body->velocity.y;
			} else {
				leastTime = min(xTime, yTime);
				intendedDX = leastTime*entityA->body->velocity.x;
				intendedDY = leastTime*entityA->body->velocity.y;
			}						
			UNUSED(intendedDX);
			UNUSED(intendedDY);	
			entityA->object->rect.x -= entityA->body->velocity.x;
			entityA->object->rect.y -= entityA->body->velocity.y;
			//setVelocity(&entityA, vecScale(entityA->body->velocity, 0));
		}

		if (entityA->collide == COLLIDE_CIRCLE && entityB->collide == COLLIDE_CIRCLE) {
			//when circles collide
			//find the collision normal between them
			//collision placement
			float radiusA = (entityA->object->rect.w+entityA->object->rect.h)/4;
			vec circleCenterA = VECCNT(entityA->object->rect.x+entityA->object->rect.w/2, entityA->object->rect.y+entityA->object->rect.h/2);

			float radiusB = (entityB->object->rect.w+entityB->object->rect.h)/4;
			vec circleCenterB = VECCNT(entityB->object->rect.x+entityB->object->rect.w/2, entityB->object->rect.y+entityB->object->rect.h/2);
			vec circleDistance = vecSub(circleCenterA, circleCenterB); // a - b -> AB
			float pushDistance = (radiusA + radiusB - vecLength(circleDistance))/2;
			//printf("%f\n", vecLength(circleDistance));
			//printf("%f\n", pushDistance);
			vec pushAB = vecScale(vecNorm(circleDistance), pushDistance);
			vec pushBA = vecScale(vecNorm(circleDistance), -pushDistance);
			entityA->object->rect.x += pushAB.x;
			entityA->object->rect.y += pushAB.y;
			entityB->object->rect.x += pushBA.x;
			entityB->object->rect.y += pushBA.y;
			UNUSED(pushBA);
			UNUSED(pushAB);
			//printf("weeweoo\n");

			//collision resolution	
		}
	}
	
		for (size_t i = 0; i < entities->key->arraySize; i++) {
		entity** intEntity = (*(entity***)getElement(entities->value, i));
		//v = u + at
		if ((*intEntity)->body == NULL) {
			continue;
		}
		if (vecLength((*intEntity)->body->acceleration) > physicsData.maxAcceleration) {
			(*intEntity)->body->acceleration = vecScale(vecNorm((*intEntity)->body->acceleration), physicsData.maxAcceleration);
		}
		//(*intEntity)->body->acceleration = vecRotate(VECCNT(0, -9.8), physicsTime*2*PI);
		vec velocity = vecAdd(vecScale((*intEntity)->body->acceleration, physicsTimeStep), (*intEntity)->body->velocity);
		if (vecLength(velocity) > physicsData.maxVelocity) {
			velocity = vecScale(vecNorm(velocity), physicsData.maxVelocity);

		}

		//printf("%f\n", velocityMagnitude);
		//velocity = vecAdd(velocity, vecScale(velocity, -0.001));


		(*intEntity)->object->rect.x += velocity.x * dt*60;
		/*if ((*intEntity)->object->rect.x + (*intEntity)->object->rect.w > SCREEN_WIDTH) {
			(*intEntity)->object->rect.x = SCREEN_WIDTH - (*intEntity)->object->rect.w;
			velocity.x = -velocity.x;
		}
		if ((*intEntity)->object->rect.x < 0) {
			(*intEntity)->object->rect.x = 0;
			velocity.x = -velocity.x;
		}*/

		(*intEntity)->object->rect.y += velocity.y * dt*60;
		/*if ((*intEntity)->object->rect.y + (*intEntity)->object->rect.h > SCREEN_HEIGHT) {
			(*intEntity)->object->rect.y = SCREEN_HEIGHT - (*intEntity)->object->rect.h;
			velocity.y = -velocity.y;
		}
			if ((*intEntity)->object->rect.y < 0) {
			(*intEntity)->object->rect.y = 0;
			velocity.y = -velocity.y;
		}*/

		//(*intEntity)->object->angle = vecAngle(velocity);
		if (vecLength(velocity) > 0) {
			//printf("%s\n", (*intEntity)->object->name);
			updateObject((*intEntity)->object);
		}
		(*intEntity)->body->velocity = velocity;
		KE += (*intEntity)->body->mass * 0.5 * vecLength(velocity)*vecLength(velocity);
	}
	}


	//printf("KE: %f\n", KE);

		/*
		entityA->object->rect.x += entityA->body->velocity.x;
		entityA->object->rect.y += entityA->body->velocity.y;
		entityB->object->rect.x += entityB->body->velocity.x;
		entityB->object->rect.y += entityB->body->velocity.y;*/
		//setVelocity(&entityA, vecScale(entityA->body->velocity, -1));
		//setVelocity(&entityB, vecScale(entityB->body->velocity, -1));
		//addForce(&entityA, vecScale(distance, 1));
		//addForce(&entityB, vecScale(distance, -1));
		//entityA->body->velocity = vecAdd(vecScale(distance, 0.5), entityA->body->velocity);
		//entityB->body->velocity = vecAdd(vecScale(distance, -0.5), entityB->body->velocity);
}

void addVelocity(entity** a, vec velocity) {
	(*a)->body->velocity = vecAdd(velocity, (*a)->body->velocity);
}

void setVelocity(entity** a, vec velocity) {
	(*a)->body->velocity = velocity;
}
void initPhysics() {
	physicsData = (universe){
		.maxForce = 2000,
		.maxVelocity = 20,
		.maxAcceleration = 2000
	};
}

float circleCircleCollision(entity** a, entity** intEntity) {
	Rect entityRect = (*a)->object->rect;
	Rect intRect = (*intEntity)->object->rect;

	float radius1 = (entityRect.w+entityRect.h)/4;
	vec circleCenter1 = VECCNT(entityRect.x+(entityRect.w/2), entityRect.y+(entityRect.h/2));

	
	float radius2 = (intRect.w+intRect.h)/4;
	vec circleCenter2 = VECCNT(intRect.x+(intRect.w/2), intRect.y+(intRect.h/2));

	vec distance = vecSub(circleCenter1, circleCenter2);
	//printf("d: %f\n", (-vecLength(distance) + (radius1 + radius2)));
	if ((vecLength(distance) - (radius1 + radius2)) <= -COLLISION_EPSILON) {
		return (vecLength(distance));
	}
	return 0.;
}

float circleBoxCollision(entity** a, entity** intEntity) {
	//search box on this, finding a circle collision
	//radius is the iterated entities' width, divided by 2
	//center is at the center of the object
	//god i fucking hate collision detection
	Rect entityRect = (*a)->object->rect;
		//impl from http://www.jeffreythompson.org/collision-detection/circle-rect.php
	Rect intRect = (*intEntity)->object->rect;
	float circleX = intRect.x + intRect.w/2;
	float circleY = intRect.y + intRect.h/2;
	float testX = circleX;
	float testY = circleY;
	float radius = (intRect.w+intRect.h)/4;

	if (circleX < entityRect.x) { 				      testX = entityRect.x;}        // left edge
	else if (circleX > entityRect.x + entityRect.w) { testX = entityRect.x + entityRect.w;}     // right edge

	if (circleY < entityRect.y) {        		      testY = entityRect.y;}       // top edge
	else if (circleY > entityRect.y + entityRect.h) { testY = entityRect.y + entityRect.h;}     // bottom edge

	vec distance = VECCNT(circleX - testX, circleY - testY);
	if ((vecLength(distance) - radius) <= -COLLISION_EPSILON) {
		return vecLength(distance);
	}

	return 0;
}

float AABBCollision(entity** a, entity** intEntity) {
	Rect rect1 = (*a)->object->rect;
	Rect rect2 = (*intEntity)->object->rect;
	if (rect1.x - rect2.x - rect2.w < -COLLISION_EPSILON &&
   		rect1.x + rect1.w - rect2.x > -COLLISION_EPSILON &&
   		rect1.y - rect2.y - rect2.w < -COLLISION_EPSILON &&
   		rect1.y + rect1.h - rect2.y > -COLLISION_EPSILON) {
			//collision!
		return 1;
   	}
		
	return 0;
}