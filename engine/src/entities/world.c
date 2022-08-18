#include "engine.h"

typedef struct {
	float spawnDt;
	float enemySpawnDt;
	entity** player;
} worldData;

typedef struct {
	int hp;
	entity** player;
	float gunDt;
	float enemyDt;
	entity** healthBar;
} enemyData;

typedef struct {
	entity** parent;
	float bulletDt;
	float aliveDt;
} bulletData;

void enemyHandler(entity** this);
void enemyCollisionHandler(entity** this, entity** a, float b);

void worldHandler(entity** this) {
	worldData* data = (worldData*)(*this)->data;
	if (rand() % 64 == 0) {
		createEntity("tile", (Rect){rand() % 1000 - 500,rand() % 1000 - 500,64,64}, 0, 0, 1.0, 0, getTexture("DEFAULT"), COLLIDE_CIRCLE,
		NULL, NULL, 0,
		collision_stub, &(body){10, VECCNT(0,0), VECCNT(0,0), VECCNT(0,0)});
	}
	int speed = 5;
	//bug here w/ camera lagging behind the player. 
	if (keyPresses[SDL_SCANCODE_W]) {
		ENTRECT(y) -= speed;
		viewport.y -= speed;
	} if (keyPresses[SDL_SCANCODE_A]) {
		ENTRECT(x) -= speed;
		viewport.x += speed;
	} if (keyPresses[SDL_SCANCODE_S]) {
		ENTRECT(y) += speed;
		viewport.y += speed;
	} if (keyPresses[SDL_SCANCODE_D]) {
		ENTRECT(x) += speed;
		viewport.x -= speed;
	}
	data->spawnDt += dt;
	//need enemy spawning

	//should they fire or swarm? 
	//aaaa
	//they should fire at the player but not try to move closer fast
	//random square spawn around player, determine this later idc
	if (data->enemySpawnDt <= data->spawnDt) {
		data->spawnDt = 0;
		vec pos = VECCNT((*data->player)->object->rect.x, (*data->player)->object->rect.y);
		pos = vecAdd(pos, randBox(1500, 1500));
		createEntity("Enemy", (Rect){pos.x, pos.y, 128, 64}, 0, 0, 1.0, 0, getTexture("Enemy"), COLLIDE_CIRCLE,
				enemyHandler, &(enemyData){.hp = 3, 
										   .player = data->player,
										   .gunDt = 1.0,
										   .enemyDt = 0.0
											}, sizeof(enemyData),
				enemyCollisionHandler, &(body){10, VECCNT(0,0), VECCNT(0,0), VECCNT(0,0)});
	}
}





void bulletHandler(entity** this) {
	bulletData* data = (bulletData*)(*this)->data;
	data->aliveDt += dt;
	if (data->aliveDt >= data->bulletDt) {
		deleteEntity(this);
	}

	(*this)->object->angle += 15;
	//rotate the movement vector each frame for funny
	//setVelocity(this, vecRotate((*this)->body->velocity, 1));
	updateObject((*this)->object);
}

void bulletCollisionHandler(entity** this, entity** a, float b) {
	bulletData* data = (bulletData*)(*this)->data;
	if ((*a) == (*data->parent)) {
		return;
	} else if (strcmp((*a)->object->name, (*this)->object->name) != 0) {
		deleteEntity(this);
	} 

	UNUSED(this);
	UNUSED(a);
	UNUSED(b);
}

typedef struct {
	float gunDt;
	float playerDt;
	int kills;
	int gunID;
	int hp;
	entity** healthBar;
} playerData;

void playerHandler(entity** this) {
	entity* player = *this;
	playerData* data = (playerData*)player->data;
	if (data->hp <= 0) {
		deleteHealthBar(data->healthBar);
		deleteEntity(this);
	}

	if (data->healthBar == NULL) {
		data->healthBar = createHealthBar(data->hp, data->hp, this);
	}
	updateHealthBar(data->hp, data->healthBar);

	int x,y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);
	x -= SCREEN_WIDTH / 2;
	y -= SCREEN_HEIGHT / 2;
	player->object->angle = vecAngle(VECCNT(x,y)) - 90;

	data->playerDt += dt;

	float speed = 5;

	setVelocity(this, VECCNT(0,0));
	if (keyPresses[SDL_SCANCODE_W]) {
		addVelocity(this, VECCNT(0, speed));
	} if (keyPresses[SDL_SCANCODE_A]) {
		addVelocity(this, VECCNT(-speed, 0));
	} if (keyPresses[SDL_SCANCODE_S]) {
		addVelocity(this, VECCNT(0, -speed));
	} if (keyPresses[SDL_SCANCODE_D]) {	
		addVelocity(this, VECCNT(speed, 0));
	}


	if ((buttons & SDL_BUTTON_LMASK) != 0) {
		switch (data->gunID) {
			case 0:
				if (data->playerDt >= data->gunDt) {
					data->playerDt = 0;
					//rotation is off center, need to add visualisations for this kinda stuff
					vec bulletMovement = vecRotate(VECCNT(0, 16), player->object->angle);
					vec rotationOrigin = VECCNT(ENTRECT(x) + ENTRECT(w)/2, ENTRECT(y) + ENTRECT(h)/2);
					vec bulletPosition = vecRotateAroundOrigin(VECCNT(ENTRECT(x)+ENTRECT(w)/2, ENTRECT(y) + ENTRECT(h)), rotationOrigin, player->object->angle);
					
					createEntity("Bullet", (Rect){bulletPosition.x, bulletPosition.y, 32, 32}, 0, 0, 1.0, player->object->angle, getTexture("Bullet1"), COLLIDE_CIRCLE, 
						bulletHandler, &(bulletData){.parent = this,
													 .bulletDt = 10,
													 .aliveDt = 0
													}, sizeof(bulletData), 
						bulletCollisionHandler, &(body){0.1, bulletMovement, VECCNT(0,0), VECCNT(0,0), vecScale(bulletMovement, 1)});

				}
				break;
			default:
				printf("no gun! lmao\n");
		}
	}


	updateObject(player->object);
}

void playerCollider(entity** this, entity** collision, float distance) {
	UNUSED(this);
	UNUSED(collision);
	UNUSED(distance);

	if (strcmp((*collision)->object->name, "Enemy") == 0) {
		enemyData* eData = (enemyData*)(*collision)->data;
		playerData* pData = (playerData*)(*this)->data;
		eData->hp--;
		pData->hp--;
	} else if (strcmp((*collision)->object->name, "Bullet") == 0) {
		bulletData* bData = (bulletData*)(*collision)->data;
		if (*bData->parent != *this) {
			playerData* pData = (playerData*)(*this)->data;
			pData->hp--;
		}
	}
}

void worldInit() {
	loadTexture("engine/data/images/newplayer.png", "Player");
	loadTexture("engine/data/images/bullet.png", "Bullet1");
	loadTexture("engine/data/images/enemybullet.png", "enemybullet");
	loadTexture("engine/data/images/giantRat.png", "Enemy");
	loadTexture("engine/data/images/health.png", "Healthbar");
	loadTexture("engine/data/images/healthback.png", "HealthbarBack");

	int id = createEntity("Player", (Rect){400 - 32,400 - 32,96,96}, 0, 0, 1.0, 0, getTexture("Player"), COLLIDE_CIRCLE,
		playerHandler, &(playerData){.gunDt = .100,
									 .playerDt = 0,
									 .kills = 0,
									 .gunID = 0, 
									 .hp = 10
												}, sizeof(playerData),
		playerCollider, &(body){10, VECCNT(0,0), VECCNT(0,0), VECCNT(0,0)});

	createEntity("World", (Rect){0,0,0,0}, 0, 0, 0.0, 0, NULL, 0,
		worldHandler, &(worldData){.spawnDt = 0,
								   .enemySpawnDt = .5,
								   .player = getEntityByID(id)
		}, sizeof(worldData),
		NULL, NULL);

}

void enemyHandler(entity** this) {
	enemyData* data = (enemyData*)(*this)->data;
	entity** player = data->player;

	if (data->hp <= 0) {
		deleteHealthBar(data->healthBar);
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
	(*this)->object->angle = vecAngle(direction) - 90;
	updateObject((*this)->object);

	direction = vecScale(vecNorm(direction), 15);
	if (data->enemyDt >= data->gunDt) {
		data->enemyDt = 0;
		createEntity("Bullet", (Rect){ENTRECT(x), ENTRECT(y), 32, 32}, 0, 0, 1.0, (*this)->object->angle, getTexture("enemybullet"), COLLIDE_CIRCLE, 
							bulletHandler, &(bulletData){.parent = this,
														 .bulletDt = 10,
														 .aliveDt = 0
														}, sizeof(bulletData), 
							bulletCollisionHandler, &(body){0.1, direction, VECCNT(0,0), VECCNT(0,0), vecScale(direction, 1)});
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


/*int createEntity(
const char* objName, Rect rect, int xOffset, int yOffset, float scale, double angle, int_Texture* texture, int collide, 
void (*entity_handler)(entity**), void* data, int dataSize, 
void (*collide_handler)(entity**, entity**, float), body* bodyData) {
*/

