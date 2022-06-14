#include "engine.h"

/**
 * @brief      Renders the objects.
 *
 * @return     Rendering status.
 */
int render() {
	renderedObjects = 0;
	
	SDL_GL_SwapWindow(window);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(objectShader.shaderProgram);
	
	
	/*for (int i = 0; i < objectUID; i++) {
		elements[i * 6 + 0] = i * 4 + 0;
		elements[i * 6 + 1] = i * 4 + 1;
		elements[i * 6 + 2] = i * 4 + 2;
		elements[i * 6 + 3] = i * 4 + 0;
		elements[i * 6 + 4] = i * 4 + 3;
		elements[i * 6 + 5] = i * 4 + 2;
	}*/

	/*for rotations, translate and then apply a rotation matrix
	add -xi - 0.5 + -yj - 0.5

	rotate
	add this back

	
	*/
	glBindTexture(GL_TEXTURE_2D, txAtlasID);
	glBindVertexArray(objectShader.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, objectShader.VBO); 
	glBufferData(GL_ARRAY_BUFFER, renderObjectSize * 16 * sizeof(*vertices), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectShader.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderObjectSize * 6 * sizeof(*elements), elements, GL_DYNAMIC_DRAW);
	GLint movement = glGetUniformLocation(objectShader.shaderProgram, "movement");
	glUniform3f(movement, (floor(viewport.x) * 2.0 + SCREEN_WIDTH) / SCREEN_WIDTH - 1.0, (floor(viewport.y) * 2.0 + SCREEN_HEIGHT) / SCREEN_HEIGHT - 1.0, 0);

	glDrawElements(GL_TRIANGLES, renderObjectSize * 6, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, objectCount * 6);

	//text rendering
 	renderText();

	return 0;
}



