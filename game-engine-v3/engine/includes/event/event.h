#pragma once
#define EVENT_EVENT_H

void processEvents();

int eventCount;
SDL_Event events[256];

const Uint8* keyPresses;