#include "engine.h"

int render() {
	SDL_GL_SwapWindow(window);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	
	dictionary intObjectDict = objects->next;
	int count = 0;
	float* vertices = gmalloc(sizeof(*vertices) * 18 * objectCount);
	while (intObjectDict != NULL) {
		object* intObject = (object*)intObjectDict->value;
		
		vertices[count * 18 + 0] = intObject->rect.x * (1.0 / SCREEN_WIDTH) - 1;  // top left
		vertices[count * 18 + 1] = intObject->rect.y * (1.0 / SCREEN_HEIGHT) - 1;

		vertices[count * 18 + 3] = (intObject->rect.x + intObject->rect.w) * (1.0 / SCREEN_WIDTH) - 1; //top right
		vertices[count * 18 + 4] = intObject->rect.y * (1.0 / SCREEN_HEIGHT) - 1;

		vertices[count * 18 + 6] = (intObject->rect.x + intObject->rect.w) * (1.0 / SCREEN_WIDTH) - 1; //bottom right
		vertices[count * 18 + 7] = (intObject->rect.y + intObject->rect.h) * (1.0 / SCREEN_HEIGHT) - 1;

		vertices[count * 18 + 9] = vertices[0]; //top left again
		vertices[count * 18 + 10] = vertices[1];

		vertices[count * 18 + 12] = intObject->rect.x * (1.0 / SCREEN_WIDTH) - 1; //bottom left
		vertices[count * 18 + 13] = (intObject->rect.y + intObject->rect.h) * (1.0 / SCREEN_HEIGHT) - 1;

		vertices[count * 18 + 15] = vertices[6]; //bottom right again
		vertices[count * 18 + 16] = vertices[7];

		intObjectDict = intObjectDict->next;
		count++;
	}
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, objectCount*6 * sizeof(*vertices), vertices, GL_DYNAMIC_DRAW);
  
	glDrawArrays(GL_TRIANGLES, 0, objectCount*6);
	gfree(vertices);
	return 0;
}