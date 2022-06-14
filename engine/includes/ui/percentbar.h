#pragma once
#define PERCENT_BAR_UI_H

typedef struct {
	RGBA colours[3];
	float amounts[3];
	object* objects[3];
	float width;
	void (*handler_func)(entity**);
} percentBarData;

void createPercentBar(RGBA* colours, Rect rect, void (*handler_func)(entity**), char* name);

void percentBarRenderer(entity** this);