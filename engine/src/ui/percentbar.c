#include "engine.h"

void percentBarRenderer(entity** this) {
	percentBarData* data = (*this)->data;
	float sigmaAmounts = 0;
	for (int i = 0; i < 3; i++) {
		sigmaAmounts += fabs(data->amounts[i]);
	}
	for (int i = 0; i < 3; i++) {
		data->objects[i]->rect.w = data->width * fabs(data->amounts[i])/sigmaAmounts;
		
		if (i != 0) {
			data->objects[i]->rect.w += data->objects[i-1]->rect.w;
		}
		if (data->objects[i]->rect.w > data->width) {
			data->objects[i]->rect.w = data->width;
		}
		updateObject(data->objects[i]);
	}
}	

void createPercentBar(RGBA* colours, Rect rect, void (*handler_func)(entity**), char* name) {
	if (colours == NULL) {
		return;
	}
	percentBarData* data = gmalloc(sizeof(percentBarData));
	data->objects[2] = createObject("percentbarunit2", (Rect){rect.x, rect.y, 0, rect.h}, 0, 0, 1.0, 0, colourTexture(colours[2]));
	data->objects[1] = createObject("percentbarunit1", (Rect){rect.x, rect.y, 0, rect.h}, 0, 0, 1.0, 0, colourTexture(colours[1]));
	data->objects[0] = createObject("percentbarunit0", (Rect){rect.x, rect.y, rect.w, rect.h}, 0, 0, 1.0, 0, colourTexture(colours[0]));
	memcpy(data->colours, colours, sizeof(RGBA) * 3);
	data->handler_func = handler_func;
	data->width = rect.w;
	for (int i = 0; i < 3; i++) {
		data->amounts[i] = 0;
	}
	createEntity(name, (Rect){0,0,0,0}, 0, 0, 0, 0.0,
		NULL, 0, handler_func, data, sizeof(*data),
		NULL, NULL
		);
	return;
}

//make this an n amount percent bar