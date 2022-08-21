#include "engine.h"

int loadSound(const char *soundDir, const char* soundName) {
	if (sounds == NULL) {return -1;}
	if (soundDir == NULL || soundDir == NULL) {
		char buffer[1024];
		sprintf(buffer, "Attempted to load sound \"%s\" at \"%s\" but one of the arguments was NULL!", soundName == NULL ? "NULL" : soundName, soundDir == NULL ? "NULL" : soundDir);
		logtofile(buffer, ERR, "Audio");
		return 1;
	}

	intSound* sound = gmalloc(sizeof(*sound));
	Mix_Music* music = Mix_LoadMUS(soundDir);
	if (music == NULL) {
		char buffer[256];
		sprintf(buffer, "Error loading sound: %s", SDL_GetError());
		logtofile(buffer, ERR, "Audio");
		return 1;
	}

	sound->sound = music;
	sound->duration = 0.0f;
	//for some reason my sdl2 doesnt have music length so once i understand sound a bit more i'll make this nicer

	

	addToDictionary(sounds, soundName, sound);
	soundCount++;
	return 0;
}

intSound* getSound(const char* key) {
	if (sounds == NULL) {return NULL;}
	intSound* sound;

	size_t soundDictIndex = findKey(sounds, key);
	if (soundDictIndex == NOVALUE) {
		return NULL;
	}

	sound = *(intSound**)getElement(sounds->value, soundDictIndex);

	return sound;
}

int playSound(intSound* sound) {
	if (sound == NULL) {return -1;}
	//only one sound can play at once, i gotta do chunks instead OR use openAL
	//i think the openAL approach here would be better, but whatever i just want something
	//printf("new audio volume: %d\n", Mix_Volume(-1, 1));
	Mix_PlayMusic(sound->sound, 0);
	return 0;
}
//this whole system is really unrobust and will crash on the slightest erroneous data, but it works for now
//replace this with OpenAL for actually nice results