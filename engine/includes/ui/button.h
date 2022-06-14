#pragma once
#define UI_BUTTON_H

typedef struct {
	dynArray* buttons;
} buttonWatchdogData;
	
buttonWatchdogData BWdata;

typedef struct {
	int_Texture* tx;
	RGBA colour;
	char* text;
	vec offset;
	float fontHeight;
	void (*trigger_func)(entity**, void*);
	void* argument;
	void (*handler_func)(entity**);
} buttonData;

void buttonWatchdog(entity** this);
void createButton(char* name, Rect buttonDim, buttonData* data);

