#pragma once
#define UI_TEXT_H

void initText();

FT_Face abyssinica;

dynArray* font;

typedef struct {
	uint txID;
	vec size;
	vec bearing;
	uint advance;
} character;

program textShader;

stack* textStack;

void renderText();
void drawText(char* text, float x, float y, float scale, RGBA colour);

typedef struct {
	char* text;
	float x;
	float y;
	float scale;
	RGBA colour;
} textStackEntry;
