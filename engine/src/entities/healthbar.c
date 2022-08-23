#include "engine.h"

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
		int id = createEntity((object){.name = buffer,
						 			  .rect = (Rect){ownerRect.x, ownerRect.y + 16, 64, 16}, 
									  .xOffset = 0,
									  .yOffset = 0,
									  .scale = 1.0,
									  .angle = 0,
									  .texture = getTexture("HealthbarBack"),
									  .layer = 1}
									  ,COLLIDE_NONE, NULL, NULL, 0, 
									  NULL, &(body){.mass = 0,
								.angularVelocity = 0.0,
								.velocity = VECCNT(0,0),
							    .netForce = VECCNT(0,0),
								.acceleration = VECCNT(0,0)});
		data->healthBarBack = getEntityByID(id);
		gfree(buffer);
	}

	if (data->healthBar == NULL) {
		char* buffer = gmalloc(strlen(data->name) + 18 + 4 + 2);
		sprintf(buffer, "HBAR-%s-%d", data->name, objectUID+1);
		int id = createEntity((object){.name = buffer,
						 			  .rect = (Rect){ownerRect.x, ownerRect.y + 16, 64, 16}, 
									  .xOffset = 0,
									  .yOffset = 0,
									  .scale = 1.0,
									  .angle = 0,
									  .texture = getTexture("Healthbar"),
									  .layer = 0}
									  ,COLLIDE_NONE, NULL, NULL, 0, 
									  NULL, &(body){.mass = 0,
								.angularVelocity = 0.0,
								.velocity = VECCNT(0,0),
							    .netForce = VECCNT(0,0),
								.acceleration = VECCNT(0,0)});
		data->healthBar = getEntityByID(id);
		gfree(buffer);
	}

	(*data->healthBar)->object->rect.x = ownerRect.x;
	(*data->healthBar)->object->rect.y = ownerRect.y + ownerRect.h;
	(*data->healthBarBack)->object->rect.x = ownerRect.x;
	(*data->healthBarBack)->object->rect.y = ownerRect.y + ownerRect.h;
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
	int id = createEntity((object){.name = buffer,
						 			  .rect = (Rect){0, 0, 0, 0}, 
									  .xOffset = 0,
									  .yOffset = 0,
									  .scale = 0.0,
									  .angle = 0,
									  .texture = getTexture("DEFAULT"),
									  .layer = 0}, COLLIDE_NONE, 
		healthBarHandler, &(healthBarData){max, health, NULL, NULL, owner, strdup(buffer)}, sizeof(healthBarData), NULL,
		&(body){.mass = 0,
								.angularVelocity = 0.0,
								.velocity = VECCNT(0,0),
							    .netForce = VECCNT(0,0),
								.acceleration = VECCNT(0,0)});

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