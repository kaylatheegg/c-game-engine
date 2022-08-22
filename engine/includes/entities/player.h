#pragma once
#define ENTITIES_PLAYER_H

typedef struct {
	float gunDt;
	float playerDt;
	int kills;
	int gunID;
	int hp;
	int maxHp;
	entity** healthBar;
	int invincibility;
	float meleeDt;
	float meleeWait;
} playerData;

void playerCollider(entity** this, entity** collision, float distance);
void playerHandler(entity** this);