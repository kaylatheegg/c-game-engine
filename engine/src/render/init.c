#include "../../includes/engine.h"

/**
 * @brief      Initializes the render.
 *
 * @return     Init status
 */
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

	//SDL_ShowCursor(0);

	return 0;
}

/**
 * @brief      Initializes the openGL renderer.
 *
 * @return     OpenGL context
 */
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

	int status = loadShaders();
	if (status != 0) {
		logtofile("Shader Error!", SVR, "Render");
		crash();
	}

	float vertices[] = {
  	  -0.5f, -0.5f, 0.0f,
  	   0.5f, -0.5f, 0.0f,
   	   0.0f,  0.5f, 0.0f
	};  
	glBindVertexArray(objectShader.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glViewport(0,0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_TEXTURE_2D); //cap error here
	glEnable(GL_BLEND);  
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
	glClearColor(1.f, 1.f, 1.f, 1.0f);
	SDL_GL_SetSwapInterval(0);
	return intContext;
}

/**
 * @brief      Destroys the renderer
 */
void cleanRender() {
	logtofile("Destroying textures and dict.", INF, "Render");
	cleanTexture();

	logtofile("Destroying renderer", INF, "Render"); 
	SDL_GL_DeleteContext(context);
	//destroyShaders();
	//freeDictionary(shaders);
	gfree(vertices);
	gfree(elements);


	logtofile("Destroying window", INF, "Render"); 
	SDL_DestroyWindow(window);
}



int loadShader(program* intProgram) {
	FILE* fp;

	if (intProgram->vertexPath == NULL) {
		logtofile("Vertex shader path is NULL!", ERR, "Render");
		return -1;
	}

	if (intProgram->fragmentPath == NULL) {
		logtofile("Fragment shader path is NULL!", ERR, "Render");
		return -1;
	}

	fp = fopen(intProgram->vertexPath, "r");
	if (fp == NULL) {
		logtofile("Vertex shader could not be found!", ERR, "Render");
		return -1;
	}

	intProgram->vertex = loadData(fp, "vertex");
	fclose(fp);

	fp = fopen(intProgram->fragmentPath, "r");
	if (fp == NULL) {
		logtofile("Fragment shader could not be found!", SVR, "Render");
		return -1;
	}
	intProgram->fragment = loadData(fp, "fragment");
	fclose(fp);

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, intProgram->vertex->lineCount, (const GLchar**)intProgram->vertex->code, NULL);
	glCompileShader(vertexShaderID);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE) {
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		*(strchr(infoLog, '\n')) = ' '; //removes the newline opengl shoves in
		logtofile("Vertex shader compilation error!", ERR, "Render");
		logtofile(infoLog, ERR, "Render");
		printf("%s\n", intProgram->vertexPath);
		return -1;
	}

	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, intProgram->fragment->lineCount, (const GLchar**)intProgram->fragment->code, NULL);
	glCompileShader(fragmentShaderID);

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE) {
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		*(strchr(infoLog, '\n')) = ' '; //removes the newline opengl shoves in
		logtofile("Fragment shader compilation error!", ERR, "Render");
		logtofile(infoLog, ERR, "Render");
		return -1;
	}

	intProgram->shaderProgram = glCreateProgram();
	glAttachShader(intProgram->shaderProgram, vertexShaderID);
	glAttachShader(intProgram->shaderProgram, fragmentShaderID);
	glLinkProgram(intProgram->shaderProgram);

	glGetProgramiv(intProgram->shaderProgram, GL_LINK_STATUS, &success);
	if(success != GL_TRUE) {
    	glGetProgramInfoLog(intProgram->shaderProgram, 512, NULL, infoLog);
    	logtofile("Program compilation error:", ERR, "Render");
		logtofile(infoLog, ERR, "Render");
		return -1;
	}


	return 0;
}

	void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
	UNUSED(source);
	UNUSED(	id);
	UNUSED(	length); 
	UNUSED(	userParam);
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}
/**
 * @brief      Loads shaders.
 *
 * @return     Shader loading status
 */
int loadShaders() {
	logtofile("Loading Shaders!", INF, "Render");

	// During init, enable debug output
	//glEnable              ( GL_DEBUG_OUTPUT );
	//glDebugMessageCallback( MessageCallback, 0 );

	objectShader = (program){.vertexPath = "engine/data/shaders/shader.vs", .fragmentPath = "engine/data/shaders/shader.fs"};
	loadShader(&objectShader);
	//printf("%d", objectShader->shaderProgram);
	
	glUseProgram(objectShader.shaderProgram);

	glGenVertexArrays(1, &objectShader.VAO); 
	glBindVertexArray(objectShader.VAO);

	glGenBuffers(1, &objectShader.VBO); 
	glBindBuffer(GL_ARRAY_BUFFER, objectShader.VBO); 

	glGenBuffers(1, &objectShader.EBO);

	GLint posAttrib = glGetAttribLocation(objectShader.shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0); 
	glEnableVertexAttribArray(posAttrib);
	
	GLint texAttrib = glGetAttribLocation(objectShader.shaderProgram, "texcoord");
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float))); //here
	glEnableVertexAttribArray(texAttrib);
	

	//text shader



	return 0;
}

/**
 * @brief      Loads the data for a shader
 *
 * @param      fp    FILE* to the file
 * @param      name  Name of the shader type
 *
 * @return     shader status
 */
shader* loadData(FILE* fp, char* name) {
	UNUSED(name);
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
	//addToDictionary(shaders, name, intShader);
	return intShader;
}

/**
 * @brief      Destroys the shaders
 *
 * @return     Destruction status.
 */
int destroyShaders() {
	if (shaders == NULL) {
		logtofile("Shaders already destroyed, returning!", WRN, "Render");
		return 0;
	}
	
	for (size_t i = 0; i < shaders->key->arraySize; i++) {
		shader* intShader = *(shader**)getElement(shaders->value, i);
		for (int i = 0; i < intShader->lineCount+1; i++) {
			gfree(intShader->code[i]);
		} 
		gfree(intShader->code);
		gfree(intShader);
	}
	return 0;
}
