#pragma once
#define ENTITY_INIT_H

typedef struct entity_int {
	object* object;
	int id;
	void (*entity_handler)(struct entity_int*);
} entity;

int entityCount;

dictionary entities;

void initEntities();