#pragma once
#define ENTITY_INIT_H

typedef struct {
	float mass;
	vec velocity;
	vec netForce;
	vec angularVelocity;
	vec acceleration;
} body;

typedef struct entity_int {
	object* object;
	int id;
	int collide;
	int deleted;
	void (*entity_handler)(struct entity_int**);
	void (*collide_handler)(struct entity_int**, struct entity_int**, float distance);
	void* data;
	body* body;
} entity;

int entityCount;
int entityUID;


dictionary entities;
int deletedCount;

void initEntities();
void stub();