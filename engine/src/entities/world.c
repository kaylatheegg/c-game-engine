#include "engine.h"

void worldHandler(entity** this) {
	worldData* data = (worldData*)(*this)->data;
	//bug here w/ camera lagging behind the player. 
	//probably to due with how velocities are handled, the viewport isnt moving w/ the physics timestep and is actually moving with
	//other stuff. fuck it new camera time AT some point

	viewport.x = -((*data->player)->object->rect.x + (*data->player)->object->rect.w / 2 - SCREEN_WIDTH / 2);
	viewport.y = -((*data->player)->object->rect.y + (*data->player)->object->rect.h / 2 - SCREEN_HEIGHT / 2);
	data->spawnDt += dt;
	data->pickupDt += dt;

	//should they fire or swarm? 
	//aaaa
	//they should fire at the player but not try to move closer fast
	//random square spawn around player, determine this later idc
	//
	//wave updating function is f(x) = 10e^0.08x, so for wave x theres f(x) enemies, kind of
	if (data->enemySpawnDt <= data->spawnDt && data->waveDt < 0 && data->enemyCount != 0) {
		data->enemyCount--;
		data->spawnDt = 0;
		vec pos = VECCNT((*data->player)->object->rect.x, (*data->player)->object->rect.y);
		vec spawnPos = randBox(3000, 3000);
		if (vecLength(spawnPos) < 1500) {
			vecScale(vecNorm(spawnPos), 1500);
		}
		pos = vecAdd(pos, spawnPos);
		createEntity((object){.name = "Enemy",
					 		 .rect = (Rect){pos.x, pos.y, 140, 40}, 
							 .xOffset = 0,
							 .yOffset = 0,
							 .scale = 1.0,
							 .angle = 0,
							 .texture = getTexture("Enemy"),
							 .layer = 2}, COLLIDE_BOX,
				enemyHandler, &(enemyData){.hp = 3, 
										   .player = data->player,
										   .gunDt = 3.0,
										   .enemyDt = 0.0,
										   .world = this
											}, sizeof(enemyData),
				enemyCollisionHandler, &(body){.mass = 10,
											   .angularVelocity = 0.0,
											   .velocity = VECCNT(0,0),
											   .netForce = VECCNT(0,0),
											   .acceleration = VECCNT(0,0)});
	}

	if (data->killedCount == (int)floor(10*pow(CONST_E, 0.08 * data->waveCount))) {
		data->killedCount = 0;
		data->waveDt = 3;
		data->waveCount++;
		data->enemyCount = 10*pow(CONST_E, 0.08 * data->waveCount);
		data->spawnDt = 1.0 - 0.02 * data->waveCount;
	}
	char buffer[200];
	sprintf(buffer, "Wave: %d\nWave progress:%d%c", data->waveCount + 1, (int)floor((data->killedCount/(10*pow(CONST_E, 0.08 * data->waveCount)) * 100)), '%');
	drawText(buffer, 300, 760, 45, (RGBA){.rgba = 0xFFFFFFFF});
	if (data->waveDt > 0) {
		sprintf(buffer, "Time until next wave: %d seconds", (int)ceil(data->waveDt));
		drawText(buffer, 300, 700, 45, (RGBA){.rgba = 0xFFFFFFFF});
		data->waveDt -= dt;
	}

	if (data->pickupSpawnDt <= data->pickupDt) {

		data->pickupDt = 0;
		vec pos = VECCNT((*data->player)->object->rect.x, (*data->player)->object->rect.y);
		pos = vecAdd(pos, randBox(1500, 1500));
		createEntity((object){.name = "Pickup",
					 		 .rect = (Rect){pos.x, pos.y, 64, 64}, 
							 .xOffset = 0,
							 .yOffset = 0,
							 .scale = 1.0,
							 .angle = 0,
							 .texture = getTexture("DEFAULT"),
							 .layer = 0}, COLLIDE_CIRCLE,
				pickupHandler, &(pickupData){.id = randRange(3)
											}, sizeof(pickupData),
				pickupCollisionHandler, &(body){.mass = 10,
											   .angularVelocity = 0.0,
											   .velocity = VECCNT(0,0),
											   .netForce = VECCNT(0,0),
											   .acceleration = VECCNT(0,0)});
	}
}

typedef struct {
	entity** player;
} iconData;

void iconHandler(entity** this) {
	iconData* data = (iconData*)(*this)->data;
	playerData* pData = (playerData*)(*data->player)->data;
	(*this)->object->rect.x = floor(((*data->player)->object->rect.x + (*data->player)->object->rect.w / 2 - SCREEN_WIDTH / 2));
	(*this)->object->rect.y = floor(((*data->player)->object->rect.y + SCREEN_HEIGHT / 2 - 16));
	updateObject((*this)->object);
	char buffer[256];
	sprintf(buffer, "   : %d\n", pData->kills);
	drawText(buffer, 0, 740, 150, (RGBA){.rgba = 0xFFFFFFFF});
	//this wiggles really hard, add viewport independence soon :)
}


void worldInit() {
	loadTexture("engine/data/images/newplayer.png", "Player");
	loadTexture("engine/data/images/bullet.png", "Bullet1");
	loadTexture("engine/data/images/enemybullet.png", "enemybullet");
	loadTexture("engine/data/images/babyRat.png", "Enemy");
	loadTexture("engine/data/images/giantRat.png", "Boss");
	loadTexture("engine/data/images/health.png", "Healthbar");
	loadTexture("engine/data/images/healthback.png", "HealthbarBack");
	loadTexture("engine/data/images/healthpickup.png", "HealthPickup");
	loadTexture("engine/data/images/shotgun.png", "Shotgun");
	loadTexture("engine/data/images/pistol.png", "Pistol");
	loadTexture("engine/data/images/wall.png", "Wall");
	loadTexture("engine/data/images/blood.png", "Blood");
	loadTexture("engine/data/images/grass.png", "Grass");
	loadTexture("engine/data/images/grasstuft.png", "Grass tuft");
	loadTexture("engine/data/images/killicon.png", "Kill_icon");
	loadTexture("engine/data/images/floor.png", "Floor");

	loadSound("engine/data/sounds/gunshot.mp3", "Gunshot");

	//lazily create the ground, we dont need anything special
	//this should be replaced, but to be honest it might just be better to fix the rendering oversight and add culling

	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 200; j++) {
			createObject("ground", (Rect){-100*32 + i*32, -100*32 + j*32, 32, 32}, 0, 0, 1, randRange(4)*90, getTexture("Grass"), 32);
		}
	}

	//generate random structure
	generateWorld();

	int id = createEntity((object){.name = "Player",
    	   					 	   .rect = (Rect){400 - 32,400 - 32,96,96}, 
    	   						   .xOffset = 0,
    	   						   .yOffset = 0,
    	   						   .scale = 1.0,
    	   						   .angle = 0,
    	   						   .texture = getTexture("Player"),
    	   						   .layer = 2}, COLLIDE_BOX,
		playerHandler, &(playerData){.gunDt = .300,
									 .playerDt = 0,
									 .kills = 0,
									 .gunID = 0, 
									 .hp = 60,
									 .maxHp = 60,
									 .invincibility = 10,
									 .meleeDt = 0,
									 .meleeWait = 0.5
												}, sizeof(playerData),
		playerCollider, &(body){.mass = 10,
								.angularVelocity = 0.0,
								.velocity = VECCNT(0,0),
							    .netForce = VECCNT(0,0),
								.acceleration = VECCNT(0,0),
								.collision_type = BODY_DYNAMIC});

	createEntity((object){.name = "World",
  				 		  .rect = (Rect){0,0,0,0}, 
  						  .xOffset = 0,
  						  .yOffset = 0,
  						  .scale = 0.0,
  						  .angle = 0,
  						  .texture = NULL,
  						  .layer = 0}, 0,
		worldHandler, &(worldData){.spawnDt = 0,
								   .enemySpawnDt = .5,
								   .player = getEntityByID(id),
								   .pickupDt = 0,
								   .pickupSpawnDt = 2.5,
								   .waveCount = 0,
								   .enemyCount = 10,
								   .waveDt = 3,
								   .killedCount = 0
		}, sizeof(worldData),
		NULL, NULL);
	createEntity((object){.name = "kill icon",
						  .rect = (Rect){0,0,64,64},
						  .xOffset = 0,
						  .yOffset = 0,
						  .scale = 1.0,
						  .angle = 0,
						  .texture = getTexture("Kill_icon"),
						  .layer = 0}, 0,
						  iconHandler, &(iconData){.player = getEntityByID(id)}, sizeof(iconData), NULL, NULL);


}



/*int createEntity(
const char* objName, Rect rect, int xOffset, int yOffset, float scale, double angle, int_Texture* texture, int collide, 
void (*entity_handler)(entity**), void* data, int dataSize, 
void (*collide_handler)(entity**, entity**, float), body* bodyData) {
*/

