#pragma once
#define RENDER_INIT_H

#define FRAGMENTATION_LIMIT 16

SDL_Window *window;
//SDL_Renderer *renderer;
SDL_GLContext* context;

SDL_Surface* textureAtlas;

GLuint VAO;
GLuint VBO;
GLuint EBO;
GLuint shaderProgram;
GLuint txAtlasID;

dictionary shaders;

typedef struct {
	GLchar** code;
	int lineCount;
} shader;

int destroyShaders();

SDL_GLContext* initOpenGLRender();

float* vertices;
GLuint* elements;


int initRender();
void cleanRender();

int loadShaders();
int loadData(FILE* fp, char* name);

