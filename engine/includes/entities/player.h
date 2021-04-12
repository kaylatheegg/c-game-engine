#pragma once
#define ENTITIES_PLAYER_H

entity** AABBCollision(entity** a);

void playerHandler(entity** this);
void playerInit();


float dtCount_player;
float dtCount_enemy;