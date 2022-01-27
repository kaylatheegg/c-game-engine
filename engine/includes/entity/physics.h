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
void addForce(entity** a, vec force);