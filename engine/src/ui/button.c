#include "engine.h"

void buttonWatchdog(entity** this) {
	UNUSED(this);
	int x = 0;
	int y = 0;
	uint32_t buttons = SDL_GetMouseState(&x, &y);
	static int activeButton;
	y = SCREEN_WIDTH - y;
	if ((buttons & SDL_BUTTON_LMASK) != 0) {
		for (size_t i = 0; i < BWdata.buttons->arraySize; i++) {
			entity** intEntity = getEntityByID(*(int*)getElement(BWdata.buttons, i));
			if (intEntity == NULL) {
				continue;
			}
			Rect buttonRect = (*intEntity)->object->rect;
			//AABB collision test
			if (buttonRect.x <= x && x <= buttonRect.x + buttonRect.w &&
				buttonRect.y <= y && y <= buttonRect.y + buttonRect.h) {
				
				buttonData* data = (buttonData*)(*intEntity)->data;
				if ((*intEntity)->object->id != activeButton) {
					//printf("entity: %d\n", (*intEntity)->object->id);
					data->trigger_func(intEntity, data->argument);
					activeButton = (*intEntity)->object->id;
				}

   			}
		}
	} else {
		activeButton = 0;
	}
}



void createButton(char* name, Rect buttonDim, buttonData* data) {
	if (name == NULL) {
		name = "NULL";
	}
	//text handling for names
	char* buffer = gmalloc(strlen(name) + strlen("Button_") + 12 + 1 + 1); //extra space for a number, the seperator and the null term
	sprintf(buffer, "Button_%d_%s", entityUID + 1, name);
	
	if (data->trigger_func == NULL) {
		data->trigger_func = stub;
	}

	//printf("%08x\n", (uint)data->colour.rgba);
	if (data->tx == NULL) {
		data->tx = colourTexture(data->colour);
	}

	int id = createEntity((object){.name = (const char*)buffer,
						  .rect = buttonDim, 
						  .xOffset = 0,
						  .yOffset = 0,
						  .scale = 0.0,
						  .angle = 0,
						  .texture = data->tx,
						  .layer = 0}, 0
						  ,data->handler_func, data, sizeof(buttonData),
		NULL, NULL);
	appendElement(BWdata.buttons, &id);
}

void deleteButton() {
}

//int createEntity(const char* objName, Rect rect, int xOffset, int yOffset, float scale, double angle, int_Texture* texture, int collide, void (*entity_handler)(entity**), void* data, int dataSize, void (*collide_handler)(entity**, entity**, float), body* bodyData) {