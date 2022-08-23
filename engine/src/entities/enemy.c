#include "engine.h"

void enemyHandler(entity** this) {
	enemyData* data = (enemyData*)(*this)->data;
	entity** player = data->player;

	if (data->hp <= 0) {
		deleteHealthBar(data->healthBar);

		playerData* pData = (playerData*)(*player)->data;
		pData->kills++;

		worldData* wData = (worldData*)(*data->world)->data;
		wData->killedCount++;

		int splatCount = randRange(10);
		for (int i = 0; i < splatCount; i++) {
			vec pos = randBox(80, 80);
			createObject("death splat", (Rect){ENTRECT(x) + ENTRECT(w)/2 + pos.x, ENTRECT(y) + ENTRECT(h)/2 + pos.y, 16, 16}, 0, 0, 1, 0, getTexture("Blood"), 32);
		}
		deleteEntity(this);
	}
	data->enemyDt += dt;

	if (data->healthBar == NULL) {
		data->healthBar = createHealthBar(data->hp, data->hp, this);
	}
	updateHealthBar(data->hp, data->healthBar);

	vec direction = vecSub(VECCNT((*player)->object->rect.x, -(*player)->object->rect.y), VECCNT(ENTRECT(x), -ENTRECT(y)));
						   
	direction = vecScale(vecNorm(direction), 4);
	direction.y *= -1;
	setVelocity(this, direction);
	//(*this)->object->angle = vecAngle(direction) - 90;
	updateObject((*this)->object);

	direction = vecScale(vecNorm(direction), 15);
	if (data->enemyDt >= data->gunDt) {
		data->enemyDt = 0;
		createEntity((object){.name = "Bullet",
  				 		 	 .rect = (Rect){ENTRECT(x), ENTRECT(y), 32, 32}, 
  						  	 .xOffset = 0,
  						  	 .yOffset = 0,
  						 	 .scale = 0.0,
  						 	 .angle = (*this)->object->angle,
  						 	 .texture = getTexture("enemybullet"),
  						 	 .layer = 0}, COLLIDE_CIRCLE, 
							bulletHandler, &(bulletData){.parent = this,
														 .bulletDt = 10,
														 .aliveDt = 0
														}, sizeof(bulletData), 
							bulletCollisionHandler, &(body){.mass = 0.1,
														.angularVelocity = 0.0,
														.velocity = direction,
														.netForce = VECCNT(0,0),
														.acceleration = VECCNT(0,0)});
	}

}

void enemyCollisionHandler(entity** this, entity** a, float b) {


	UNUSED(b);
	if (strcmp("Bullet", (*a)->object->name) == 0) {
		bulletData* bData = (bulletData*)(*a)->data;
		enemyData* eData = (enemyData*)(*this)->data;
		if (*bData->parent != *this) {
			eData->hp--;
		}

	}

}
