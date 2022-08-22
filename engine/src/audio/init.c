#include "engine.h"

void initAudio() {
	if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0) {
		char buffer[256];
		sprintf(buffer, "SDL_Mixer init error: %s", SDL_GetError());
		logtofile(buffer, SVR, "Audio");
		crash();
	}
	sounds = createDictionary();
	soundCount = 0;

	Mix_OpenAudio(48000, AUDIO_F32SYS, 2, 2048);
}

