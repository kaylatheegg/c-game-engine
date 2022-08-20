#pragma once
#define AUDIO_INIT_H

void initAudio();

dictionary sounds;

//i dont rly know how sdl2 audio works so i hope this works??
//this should be chunks but for now music is fine
typedef struct {
	Mix_Music* sound;
	float duration;
} intSound;

int soundCount;

intSound* getSound(const char* key);
int loadSound(const char *soundDir, const char* soundName);
int playSound(intSound* sound);