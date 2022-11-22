#pragma once
#define ENTITY_INIT_H

enum body_type {
	BODY_STATIC = 0,
	BODY_DYNAMIC = 1
};

typedef struct {
	float mass;
	float angularVelocity;
	vec velocity;
	vec netForce;
	vec acceleration;
	enum body_type collision_type;
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