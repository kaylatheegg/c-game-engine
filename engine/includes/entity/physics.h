#pragma once
#define ENTITY_PHYSICS_H

typedef struct {
	float maxVelocity;
	float maxAcceleration;
	float maxForce;
} universe;

universe physicsData;

void initPhysics();
void processPhysics();
void addVelocity(entity** a, vec velocity);
void setVelocity(entity** a, vec velocity);

#define COLLISION_EPSILON 0.1

float AABBCollision(entity** a, entity** intEntity);
float circleCircleCollision(entity** a, entity** intEntity);
float circleBoxCollision(entity** a, entity** intEntity);