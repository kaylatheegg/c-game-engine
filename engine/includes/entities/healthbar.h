#pragma once
#define ENTITIES_HEALTHBAR_H

entity** createHealthBar(float max, float health, entity** owner);
void deleteHealthBar(entity** bar);
void updateHealthBar(float health, entity** bar);
