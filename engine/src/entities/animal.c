#include "engine.h"

typedef struct {
	int maxHealth;
	int health;
	int speed;
	int hunger;
} animal;

void animalHandler(entity** this) {
	animal* entData = (animal*)(*this)->data;


	int direction = rand() % 4;
	int speed = rand() % entData->speed * 3;
	if (speed > 16) {
		speed = 16;
		entData->speed -= 1;
	} 
	if (entData->maxHealth > 8) {
		entData->maxHealth = 8;
	} 

	if (entData->hunger == 0) {
		if (rand() % 30 == 0) {
			entData->health--;
		}
	}


	if (direction == 0) {ENTRECT(x) += speed;}
	if (direction == 1) {ENTRECT(x) -= speed;}
	if (direction == 2) {ENTRECT(y) += speed;}
	if (direction == 3) {ENTRECT(y) -= speed;}

	if (rand() % 600 == 1) {
		entData->health--;
	}
	
	if (rand() % 3 == 1) {
		entData->hunger--;
	}
	

	entity** intEntity = AABBCollision(this);
	if (intEntity != NULL) {
		if (strcmp((*intEntity)->object->name, "tile") == 0) {
			tileData* data = (tileData*)(*intEntity)->data;
			
			if (entData->hunger == 0 && entData->health < 5 && data->type == GRASS) {
				data->type = DIRT;
				world[data->x][data->y].type = DIRT;

				entData->health += rand() % 3 + 1;
				entData->hunger = 60;
			}

			if (entData->health == 0) {
				deleteEntity(this);
				data->type = GRASS;
				world[data->x][data->y].type = GRASS;
				return;
			}

			if (data->type == FIRE) {
				deleteEntity(this);
				return;
			}

			if (data->type == GRASS && entData->hunger == 0) {
				data->type = DIRT;
				world[data->x][data->y].type = DIRT;

				entData->health += rand() % 3 + 1;
				if (entData->health > entData->maxHealth) {
					entData->health = entData->maxHealth;
				}

				entData->hunger = 60;

				if (rand() % 3 == 1) {
					int newHealth = entData->maxHealth += 2 - rand() % 4;
					createEntity("animal", (SDL_Rect){ENTRECT(x),ENTRECT(y), 32, 32}, 0, 0, 1.0, 0, getTexture("Player"), 0, animalHandler, &(animal){newHealth, newHealth, entData->speed += 2 - rand() % 3, 60}, sizeof(animal));
				}
			}

		}
	} 
	updateObject((*this)->object);
}

void initAnimals() {
	for (int i = 0; i < 5; i++) {
		createEntity("animal", (SDL_Rect){800,800, 32, 32}, 0, 0, 1.0, 0, getTexture("Player"), 0, animalHandler, &(animal){15, 15, 3, 60}, sizeof(animal));
	}
}