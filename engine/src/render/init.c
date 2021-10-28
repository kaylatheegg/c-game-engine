#include "../../includes/engine.h"

int initRender() {
	char error[256];
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	logtofile("Initialising window", INF, "Render"); 
	window = SDL_CreateWindow("Game Engine",
							  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
							  SCREEN_WIDTH, SCREEN_HEIGHT,
							  SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (!window) {
    	sprintf(error, "Initialising window failure!: %s", SDL_GetError());
    	logtofile(error, SVR, "Render");
    	crash();
	}

	logtofile("Initialising renderer", INF, "Render"); 
	context = initOpenGLRender();
	if (!context) {
    	sprintf(error, "Initialising renderer failure!: %s", SDL_GetError());
    	logtofile(error, SVR, "Render");
    	crash();
	}


	/*renderer = SDL_CreateRenderer(window,
								  -1,RENDERER_ACCELER
								  SDL_ATED);
	if (!renderer) {
    	sprintf(error, "Initialising renderer failure!: %s", SDL_GetError());
    	logtofile(error, SVR, "Render");
    	crash();
	}*/

	viewport.x = 0;
	viewport.y = 0;
	viewport.w = SCREEN_WIDTH;
	viewport.h = SCREEN_HEIGHT;



	return 0;
}

SDL_GLContext* initOpenGLRender() {
	SDL_GLContext* intContext = SDL_GL_CreateContext(window);
	
	logtofile("Initialising GLEW", INF, "Render");
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		sprintf(error, "GLEW initialisation failure, error: %s\n", glewGetErrorString(err));
		logtofile(error, SVR, "Render");
		crash();
	}


	glGenVertexArrays(1, &VAO); 
	glGenBuffers(1, &VBO); 
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	float vertices[] = {
  	  -0.5f, -0.5f, 0.0f,
  	   0.5f, -0.5f, 0.0f,
   	   0.0f,  0.5f, 0.0f
	};  


	
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	int status = loadShaders();
	if (status != 0) {
		logtofile("Shader Error!", SVR, "Render");
		crash();
	}

	glViewport(0,0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);  
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	SDL_GL_SetSwapInterval(0);
	return intContext;
}

void cleanRender() {
	logtofile("Destroying textures and dict.", INF, "Render");
	cleanTexture();

	logtofile("Destroying renderer", INF, "Render"); 
	SDL_GL_DeleteContext(context);
	destroyShaders();
	freeDictionary(shaders);
	gfree(vertices);
	gfree(elements);


	logtofile("Destroying window", INF, "Render"); 
	SDL_DestroyWindow(window);
}

int loadShaders() {
	logtofile("Loading Shaders!", INF, "Render");
	shaders = createDictionary();
	FILE* fp;
	fp = fopen("engine/data/shaders/shader.vs", "r");
	if (fp == NULL) {
		logtofile("Vertex shader could not be found!", SVR, "Render");
		crash();
	}

	loadData(fp, "vertex");

	fp = fopen("engine/data/shaders/shader.fs", "r");
	if (fp == NULL) {
		logtofile("Fragment shader could not be found!", SVR, "Render");
		crash();
	}

	loadData(fp, "fragment");

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	dictionary vertexDict = findKey(shaders, "vertex");
	shader* vertexShader = (shader*)vertexDict->value;
	glShaderSource(vertexShaderID, vertexShader->lineCount, (const GLchar**)vertexShader->code, NULL);
	glCompileShader(vertexShaderID);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE) {
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		logtofile("VShader compilation error:", ERR, "Render");
		logtofile(infoLog, ERR, "Render");
		crash();
	}

	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	dictionary fragmentDict = findKey(shaders, "fragment");
	shader* fragmentShader = (shader*)fragmentDict->value;
	glShaderSource(fragmentShaderID, fragmentShader->lineCount, (const GLchar**)fragmentShader->code, NULL);
	glCompileShader(fragmentShaderID);

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE) {
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		logtofile("FShader compilation error:", ERR, "Render");
		logtofile(infoLog, ERR, "Render");
		crash();
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShaderID);
	glAttachShader(shaderProgram, fragmentShaderID);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(success != GL_TRUE) {
    	glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    	logtofile("Program compilation error:", ERR, "Render");
		logtofile(infoLog, ERR, "Render");
		crash();
	}


	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
	glEnableVertexAttribArray(posAttrib);

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));




	return 0;
}

int loadData(FILE* fp, char* name) {
	int chunkSize = 256;

	shader* intShader = gmalloc(sizeof(*intShader));

	intShader->code = gmalloc(sizeof(intShader->code));
	intShader->code[0] = gmalloc(sizeof(*intShader->code) * chunkSize);
	intShader->lineCount = 0;

	while(fgets(intShader->code[intShader->lineCount], chunkSize, fp) != NULL) {
		intShader->lineCount++;
		intShader->code = grealloc(intShader->code, sizeof(intShader->code) * (intShader->lineCount + 1));
		intShader->code[intShader->lineCount] = gmalloc(sizeof(*intShader->code) * chunkSize);
	}

	addToDictionary(shaders, name, intShader);
	return 0;
}

int destroyShaders() {
	if (shaders == NULL) {
		logtofile("Shaders already destroyed, returning!", WRN, "Render");
		return 0;
	}
	dictionary intDict = shaders->next;
	while (intDict != NULL) {
		shader* intShader = (shader*)intDict->value;
		for (int i = 0; i < intShader->lineCount+1; i++) {
			gfree(intShader->code[i]);
		} 
		gfree(intShader->code);
		gfree(intShader);
		intDict = intDict->next;
	}
	return 0;
}