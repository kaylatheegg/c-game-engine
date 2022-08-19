#pragma once
#define RENDER_INIT_H

#define FRAGMENTATION_LIMIT 16

typedef struct {
	float x,y,w,h;
} Rect;

SDL_Window *window;
//SDL_Renderer *renderer;
SDL_GLContext* context;

SDL_Surface* textureAtlas;
GLuint txAtlasID;

dictionary shaders;

typedef struct {
	GLchar** code;
	int lineCount;
} shader;

int destroyShaders();

SDL_GLContext* initOpenGLRender();

float** vertices;
GLuint** elements;


int initRender();
void cleanRender();

int loadShaders();
shader* loadData(FILE* fp, char* name);



typedef union {
	uint32_t rgba;
	struct {
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	};
} RGBA; //this might have a bug with endianness, beware!

typedef struct {
	char* vertexPath;
	char* fragmentPath;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLuint shaderProgram;
	shader* vertex;
	shader* fragment;
} program;

int loadShader(program* intProgram);

/*program objectShader = {
	.vertexPath = "engine/data/shaders/shader.vs",
	.fragmentPath = "engine/data/shaders/shader.fs",
};*/