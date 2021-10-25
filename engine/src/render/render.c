#include "engine.h"

int render() {
	renderedObjects = 0;
	SDL_GL_SwapWindow(window);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	
	dictionary intObjectDict = objects->next;
	int count = 0;
	float* vertices = gmalloc(sizeof(*vertices) * 16 * objectCount);
	GLuint* elements = gmalloc(sizeof(*elements) * 6 * objectCount);
	while (intObjectDict != NULL) {
		object* intObject = (object*)intObjectDict->value;
		/*if (intObject->rect.x + viewport.x  > SCREEN_WIDTH || intObject->rect.x + viewport.x + intObject->rect.w < 0 ||
			intObject->rect.y + viewport.y > SCREEN_HEIGHT || intObject->rect.y + viewport.y + intObject->rect.h < 0) {
			intObjectDict = intObjectDict->next;
			continue;
		}*/
		renderedObjects++;
		elements[count * 6 + 0] = count * 4 + 0;
		elements[count * 6 + 1] = count * 4 + 1;
		elements[count * 6 + 2] = count * 4 + 2;
		elements[count * 6 + 3] = count * 4 + 0;
		elements[count * 6 + 4] = count * 4 + 3;
		elements[count * 6 + 5] = count * 4 + 2;
  
		/*float vertices[] = {
  			-0.5f, -0.5f, x, y,
  			0.5f, -0.5f, x, y,
   			0.0f,  0.5f, x, y,
   			-0.5f, -0.5f, x, y,
		};*/  


		vertices[count * 16 + 0] = intObject->rect.x * 2.0 / SCREEN_WIDTH - 1.0;  // top left
		vertices[count * 16 + 1] = intObject->rect.y * 2.0 / SCREEN_HEIGHT - 1.0;
		vertices[count * 16 + 2] = 0.0f; // top left texcoord
		vertices[count * 16 + 3] = 1.0f;

		vertices[count * 16 + 4] = (intObject->rect.x + intObject->rect.w) * 2.0 / SCREEN_WIDTH - 1.0; //top right
		vertices[count * 16 + 5] = intObject->rect.y * 2.0 / SCREEN_HEIGHT - 1.0;
		vertices[count * 16 + 6] = 1.0f; // top right texcoord
		vertices[count * 16 + 7] = 1.0f;		

		vertices[count * 16 + 8] = (intObject->rect.x + intObject->rect.w) * 2.0 / SCREEN_WIDTH - 1.0; //bottom right
		vertices[count * 16 + 9] = (intObject->rect.y + intObject->rect.h) * 2.0 / SCREEN_HEIGHT - 1.0;
		vertices[count * 16 + 10] = 1.0f; // bottom right texcoord
		vertices[count * 16 + 11] = 0.0f;

		vertices[count * 16 + 12] = intObject->rect.x * 2.0 / SCREEN_WIDTH - 1.0; //bottom left
		vertices[count * 16 + 13] = (intObject->rect.y + intObject->rect.h) * 2.0 / SCREEN_HEIGHT - 1.0;
		vertices[count * 16 + 14] = 0.0f; // bottom left texcoord
		vertices[count * 16 + 15] = 0.0f;

		intObjectDict = intObjectDict->next;
		count++;
	}
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, objectCount * 16 * sizeof(*vertices), vertices, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, objectCount * 6 * sizeof(*elements), elements, GL_STATIC_DRAW);

	glEnable(GL_TEXTURE_2D);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	SDL_Surface* intSurface = getTexture("Sand")->surface;
	int mode = intSurface->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB; 

	glTexImage2D(GL_TEXTURE_2D, 0, mode, intSurface->w, intSurface->h, 0, mode, GL_UNSIGNED_BYTE, intSurface->pixels);
 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLint movement = glGetUniformLocation(shaderProgram, "movement");
	glUniform3f(movement, viewport.x * 2.0 / SCREEN_WIDTH - 1.0, viewport.y * 2.0 / SCREEN_HEIGHT - 1.0, 0.0f);

	glDrawElements(GL_TRIANGLES, objectCount * 6, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, objectCount * 6);
	gfree(vertices);
	gfree(elements);
	glBindVertexArray(0);
	return 0;
}