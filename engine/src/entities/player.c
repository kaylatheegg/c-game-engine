#include "engine.h"

void playerHandler(entity** this) {
	entity* player = *this;
	playerData* data = (playerData*)player->data;
	if (data->hp <= 0) {
		if (data->healthBar == NULL) {
			return;
		}
		deleteHealthBar(data->healthBar);
		(*this)->object->scale = 0.0;
		(*this)->object->xOffset = 17000;
		updateObject((*this)->object);
		data->healthBar = NULL;
		setVelocity(this, VECCNT(0,0));
		(*this)->collide = COLLIDE_NONE;
		return;
	}
	if (data->hp > data->maxHp) {
		data->hp = data->maxHp;
	}
	if (data->healthBar == NULL) {
		data->healthBar = createHealthBar(data->hp, data->hp, this);
	}
	updateHealthBar(data->hp, data->healthBar);

	if (data->targetBody == NULL) {
		data->targetBody = cpSpaceAddBody(space, cpBodyNew(1000000000000000, INFINITY));
		cpBodySetPosition(data->targetBody, cpv((*this)->object->rect.x, (*this)->object->rect.y));
	}
	if (data->movementConstraint == NULL) {
		data->movementConstraint = cpPivotJointNew2(data->targetBody, (*this)->body->body, cpvzero, cpvzero);
		cpConstraintSetMaxBias(data->movementConstraint,  3000.f);
		cpConstraintSetMaxForce(data->movementConstraint, 4000.f);
		cpSpaceAddConstraint(space, data->movementConstraint);
	}

	int x,y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);
	x -= SCREEN_WIDTH / 2;
	y -= SCREEN_HEIGHT / 2;
	//player->object->angle = vecAngle(VECCNT(x,y)) - 90;

	data->playerDt += dt;

	float speed = 150*60;
	cpBodySetAngle((*this)->body->body, 0);

	cpVect vp = cpBodyGetPosition((*this)->body->body);
	if (keyPresses[SDL_SCANCODE_W]) {
		vp.y += speed;
	} if (keyPresses[SDL_SCANCODE_A]) {
		vp.x -= speed;
	} if (keyPresses[SDL_SCANCODE_S]) {
		vp.y -= speed;
	} if (keyPresses[SDL_SCANCODE_D]) {	
		vp.x += speed;
	}
	cpShapeSetFriction((*this)->body->shape, 0.1);
	cpBodySetPosition(data->targetBody, vp);

	if ((buttons & SDL_BUTTON_LMASK) != 0) {
		if (data->playerDt >= data->gunDt) {
			data->playerDt = 0;
			playSound(getSound("Gunshot"));
			switch (data->gunID) {
				case 0: {
					
					//rotation is off center, need to add visualisations for this kinda stuff
					vec bulletMovement = vecRotate(VECCNT(0, 16), player->object->angle);
					vec rotationOrigin = VECCNT(ENTRECT(x) + ENTRECT(w)/2, ENTRECT(y) + ENTRECT(h)/2);
					vec bulletPosition = vecRotateAroundOrigin(VECCNT(ENTRECT(x)+ENTRECT(w)/2, ENTRECT(y) + ENTRECT(h)), rotationOrigin, player->object->angle);
					
					createEntity((object){.name = "Bullet",
			   					 		  .rect = (Rect){bulletPosition.x, bulletPosition.y, 32, 32}, 
			   							  .xOffset = 0,
			   							  .yOffset = 0,
			   							  .scale = 1.0,
			   							  .angle = player->object->angle,
			   							  .texture = getTexture("Bullet1"),
			   							  .layer = 30}, COLLIDE_BOX, 
						bulletHandler, &(bulletData){.parent = this,
													 .bulletDt = 10,
													 .aliveDt = 0
													}, sizeof(bulletData), 
						bulletCollisionHandler, &(body){.mass = 0.1,
														.angularVelocity = 0.0,
														.velocity = bulletMovement,
														.netForce = VECCNT(0,0),
														.acceleration = vecScale(bulletMovement, 1)});

				
					break;
				}
				case 1: {
					for (int i = 0; i < 5; i++) {
						vec bulletMovement = vecRotate(VECCNT(0, 32), player->object->angle - 30 + randRange(60));
						vec rotationOrigin = VECCNT(ENTRECT(x) + ENTRECT(w)/2, ENTRECT(y) + ENTRECT(h)/2);
						vec bulletPosition = vecRotateAroundOrigin(VECCNT(ENTRECT(x)+ENTRECT(w)/2, ENTRECT(y) + ENTRECT(h)), rotationOrigin, player->object->angle);
					
					createEntity((object){.name = "Bullet",
			   					 		  .rect = (Rect){bulletPosition.x, bulletPosition.y, 24, 24}, 
			   							  .xOffset = 0,
			   							  .yOffset = 0,
			   							  .scale = 1.0,
			   							  .angle = player->object->angle,
			   							  .texture = getTexture("Bullet1"),
			   							  .layer = 30}, COLLIDE_BOX, 
						bulletHandler, &(bulletData){.parent = this,
													 .bulletDt = 10,
													 .aliveDt = 0
													}, sizeof(bulletData), 
						bulletCollisionHandler, &(body){.mass = 0.1,
														.angularVelocity = 0.0,
														.velocity = bulletMovement,
														.netForce = VECCNT(0,0),
														.acceleration = vecScale(bulletMovement, 1)});
					}
					break;
				}
			default:
				printf("no gun! lmao\n");
			}
		}
	}

	char buffer[256];
	sprintf(buffer, "Current weapon: %s\n", data->gunID == 0 ? "Pistol":"Shotgun");
	drawText(buffer, 0, 650, 70, (RGBA){.rgba = 0xFFFFFFFF});
	updateObject(player->object);
	data->invincibility--;
}

void playerCollider(entity** this, entity** collision, float distance) {
	UNUSED(this);
	UNUSED(collision);
	UNUSED(distance);
	if (this == NULL || collision == NULL) {
		crash();
	}
	playerData* pData = (playerData*)(*this)->data;

	if (pData->invincibility <= 0) {
		if (strcmp((*collision)->object->name, "Enemy") == 0) {	
			pData->hp--;
			pData->invincibility = 15;
		} else if (strcmp((*collision)->object->name, "Bullet") == 0) {
			bulletData* bData = (bulletData*)(*collision)->data;
			if (*bData->parent != *this) {
				pData->hp--;
				pData->invincibility = 15;
			}
		}
	}
}