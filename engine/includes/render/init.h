#pragma once
#define RENDER_INIT_H

SDL_Window *window;
//SDL_Renderer *renderer;
SDL_GLContext* context;
GLuint VAO;
GLuint VBO;
GLuint shaderProgram;

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

