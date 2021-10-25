#pragma once
#define RENDER_TEXTURE_H

int textureCount;

const char** textureLocations;

dictionary textures;

void printTextures();
int loadTexture(const char *textureDir, const char* textureName);
void cleanTexture();
SDL_Surface* getTexture(const char* key);