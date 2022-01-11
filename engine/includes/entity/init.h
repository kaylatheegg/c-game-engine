#pragma once
#define ENTITY_INIT_H

typedef struct entity_int {
	object* object;
	int id;
	int collide;
	int deleted;
	void (*entity_handler)(struct entity_int**);
	void* data;
} entity;

int entityCount;
int entityUID;


dictionary entities;
int deletedCount;

void initEntities();