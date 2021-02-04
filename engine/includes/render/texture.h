#pragma once
#define RENDER_TEXTURE_H

int textureCount;

char** textureLocations;

dictionary textures;

void printTextures();
int loadTexture(char *textureDir, char* textureName);
void cleanTexture();
SDL_Texture* getTexture(char* key);