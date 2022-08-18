#pragma once
#define ENTITIES_H

void worldInit();

entity** createHealthBar(float max, float health, entity** owner);
void deleteHealthBar(entity** bar);
void updateHealthBar(float health, entity** bar);