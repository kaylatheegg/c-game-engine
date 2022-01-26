#include "engine.h"

//fix crashing bugs with health bar, valgrind has been no help

typedef struct {
	float max;
	float health;
	entity** healthBar;
	entity** healthBarBack;
	entity** owner;
	const char* name;
} healthBarData;

void healthBarHandler(entity** this) {
	healthBarData* data = (*this)->data;
	if (data->name == NULL) {
		data->name = strdup("NULLWasProvidedForName");
	}

	if (data->owner == NULL) {
		deleteEntity(this);
		return;
	}

	if (*(data->owner) == NULL) {
		deleteEntity(this);
		return;
	}

	Rect ownerRect = (*data->owner)->object->rect;

	if (data->healthBarBack == NULL) {
		char* buffer = gmalloc(strlen(data->name) + 18 + 4 + 2);
		sprintf(buffer, "HBAK-%s-%d", data->name, objectUID+1);
		int id = createEntity(buffer, (Rect){ownerRect.x, ownerRect.y + 16, 64, 16}, 0, 0, 1.0, 0, getTexture("HealthbarBack"), COLLIDE_NONE, 
			NULL, NULL, 0, NULL,
			&(body){00, VECCNT(0,0), VECCNT(0,0), VECCNT(0,0)});
		
		data->healthBarBack = getEntityByID(id);
		gfree(buffer);
	}

	if (data->healthBar == NULL) {
		char* buffer = gmalloc(strlen(data->name) + 18 + 4 + 2);
		sprintf(buffer, "HBAR-%s-%d", data->name, objectUID+1);
		int id = createEntity(buffer, (Rect){ownerRect.x, ownerRect.y + 16, 64, 16}, 0, 0, 1.0, 0, getTexture("Water"), COLLIDE_NONE, 
			NULL, NULL, 0, NULL,
			&(body){0, VECCNT(0,0), VECCNT(0,0), VECCNT(0,0)});

		data->healthBar = getEntityByID(id);
		gfree(buffer);
	}

	(*data->healthBar)->object->rect.x = ownerRect.x;
	(*data->healthBar)->object->rect.y = ownerRect.y + ownerRect.h;
	(*data->healthBarBack)->object->rect.x = 0;
	(*data->healthBarBack)->object->rect.y = 0;
	updateObject((*data->healthBar)->object);
	updateObject((*data->healthBarBack)->object);
	(*data->healthBar)->object->rect.w = data->health/data->max * 64;
}

void deleteHealthBar(entity** bar) {
	healthBarData* data = (healthBarData*)(*bar)->data;
	if (data == NULL) {
		return;
	}
	gfree((char*)data->name);
	if (data->healthBar != NULL) {
		deleteEntity(data->healthBar);
	}
	if (data->healthBarBack != NULL) {
		deleteEntity(data->healthBarBack);
	}
	deleteEntity(bar);
}

entity** createHealthBar(float max, float health, entity** owner) {
	if (owner == NULL) {
		return NULL;
	}

	if ((*owner) == NULL) {
		return NULL;
	}

	char* buffer = gmalloc(strlen((*owner)->object->name) + 18 + 4 + 3);
	sprintf(buffer, "HOWN-%s-%d", (*owner)->object->name, objectUID+1);
	int id = createEntity(buffer, (Rect){0, 0, 0, 0}, 0, 0, 1.0, 0, getTexture("DEFAULT"), COLLIDE_NONE, 
		healthBarHandler, &(healthBarData){max, health, NULL, NULL, owner, strdup(buffer)}, sizeof(healthBarData), NULL,
		&(body){0, VECCNT(0,0), VECCNT(0,0), VECCNT(0,0)});

	gfree(buffer);
	return getEntityByID(id);
}

void updateHealthBar(float health, entity** bar) {
	if (bar == NULL) {
		return;
	}
	if ((*bar) == NULL) {
		return;
	}

	healthBarData* data = (healthBarData*)(*bar)->data;
	if (data == NULL) {
		return;
	}
	data->health = health;
}