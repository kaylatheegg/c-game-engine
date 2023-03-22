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
/*	glBindTexture(GL_TEXTURE_2D, txAtlasID);
	glBindVertexArray(objectShader.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, objectShader.VBO); 
	glBufferData(GL_ARRAY_BUFFER, renderObjectSize * 16 * sizeof(*vertices), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectShader.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderObjectSize * 6 * sizeof(*elements), elements, GL_DYNAMIC_DRAW);
	GLint movement = glGetUniformLocation(objectShader.shaderProgram, "movement");
	glUniform3f(movement, (floor(viewport.x) * 2.0 + SCREEN_WIDTH) / SCREEN_WIDTH - 1.0, (floor(viewport.y) * 2.0 + SCREEN_HEIGHT) / SCREEN_HEIGHT - 1.0, 0);

	glDrawElements(GL_TRIANGLES, renderObjectSize * 6, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, objectCount * 6);*/

	//ok so lets do this for layers, like properly
	//this is kinda hacky, but it offloads calculative stress for objects onto the engine and not the renderer,
	//leading to less slowdown than normal
	//frankly this should be on a seperate thread, but multithreading in C is like
	//trying to use a dremel as toilet paper

    //rendering slow af but this works for now

    /* matrix projections below:
		V_viewed = M_ortho * M_camera * V_vertex
    */

  //  matrix4 ortho = perspectiveMatrix(0, 0.5, 0, 0.5, 0, 15);
    matrix4 camera = translationMatrix((floor(vecCam.x) * 2.0 + SCREEN_WIDTH) / SCREEN_WIDTH - 1.0, (floor(vecCam.y) * 2.0 + SCREEN_WIDTH) / SCREEN_WIDTH - 1.0, 0.5);
   // 
    matrix4 transMatrix = camera;

    matrix4 I = identity();
    transMatrix = I;
   // transMatrix = camera;
	for (int i = 0; i < MAX_RENDER_LAYERS; i++) {
		renderCounts[i] = 0;
	}


	for (size_t i = 0; i < objects->key->arraySize; i++) {
		object* intObject = *(object**)getElement(objects->value, i);
		
		//test if the object should actually be added
		Rect intRect = intObject->rect;
		if (vecCam.x + intRect.x - intRect.w > SCREEN_WIDTH  || vecCam.x + intRect.x + intRect.w < 0 ||
			vecCam.y + intRect.y - intRect.h > SCREEN_HEIGHT || vecCam.y + intRect.y + intRect.h < 0) {
			if (intObject->layer != 0) {
				continue;
			}
		}

		//we need to now copy over the vertex and element data
		int layer = intObject->layer;
		int count = renderCounts[layer];

		renderVertices[layer] = grealloc(renderVertices[layer], sizeof(**renderVertices) * (renderCounts[layer]+1) * 16);
		renderElements[layer] = grealloc(renderElements[layer], sizeof(**renderElements) * (renderCounts[layer]+1) * 6);

		for (int j = 0; j < 16; j++) {
			renderVertices[layer][count * 16 + j] = vertices[layer][(int)floor(intObject->vertexID.x) * 16 + j];
		}
		
		renderElements[layer][count * 6 + 0] = count * 4 + 0;
		renderElements[layer][count * 6 + 1] = count * 4 + 1;
		renderElements[layer][count * 6 + 2] = count * 4 + 2;
		renderElements[layer][count * 6 + 3] = count * 4 + 0;
		renderElements[layer][count * 6 + 4] = count * 4 + 3;
		renderElements[layer][count * 6 + 5] = count * 4 + 2;
			//printf("%d\n", renderElements[layer][renderCounts[layer] * 6 + j]);

		renderCounts[layer]++;
	}
	glBindTexture(GL_TEXTURE_2D, txAtlasID);
	for (int i = MAX_RENDER_LAYERS - 1; i >= 0; i--) {
		if (renderCounts[i] != 0) {
			if (i == 0) {
				transMatrix = I;
			} else {
				transMatrix = camera;
			}
			glBindVertexArray(objectShader.VAO);
			glBindBuffer(GL_ARRAY_BUFFER, objectShader.VBO); 
			glBufferData(GL_ARRAY_BUFFER, renderCounts[i] * 16 * sizeof(**renderVertices), renderVertices[i], GL_DYNAMIC_DRAW);	

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectShader.EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderCounts[i] * 6 * sizeof(**renderElements), renderElements[i], GL_DYNAMIC_DRAW);
			//GLint movement = glGetUniformLocation(objectShader.shaderProgram, "movement");
			//glUniform3f(movement, (floor(viewport.x) * 2.0 + SCREEN_WIDTH) / SCREEN_WIDTH - 1.0, (floor(viewport.y) * 2.0 + SCREEN_HEIGHT) / SCREEN_HEIGHT - 1.0, 0);	
			glUniformMatrix4fv(glGetUniformLocation(objectShader.shaderProgram, "transMatrix"), 1, GL_FALSE, &transMatrix.values[0][0]);
			//glUniform3f(glGetUniformLocation(textShader.shaderProgram, "textColor"), colour.r, colour.g, colour.b);
			glDrawElements(GL_TRIANGLES, renderCounts[i] * 6, GL_UNSIGNED_INT, 0);
			//glDrawArrays(GL_TRIANGLES, 0, objectCount * 6);
		}
	}

	//draw 


	/*for (size_t i = 0; i < objects->key->arraySize; i++) {
		object* intObject = *(object**)getElement(objects->value, i);

		float vertexs[16];

		float textureX = intObject->texture->x;
		float textureY = intObject->texture->y;
		float textureW = intObject->texture->surface->w;
		float textureH = intObject->texture->surface->h;
		float atlasH = textureAtlas->h;
		float atlasW = textureAtlas->w;

	
		Rect intRect = intObject->rect;
		intRect.x += intObject->xOffset;
		intRect.y += intObject->yOffset;
		int count = 0;
		if (viewport.x + intRect.x > SCREEN_WIDTH || viewport.x + intRect.x < 0 ||
			viewport.y + intRect.y > SCREEN_HEIGHT || viewport.y + intRect.y < 0) {
			continue;
		}

		vertexs[count * 16 + 2] = (textureX + 0.5) / atlasW; // top left texcoord
		vertexs[count * 16 + 3] = (textureY - 0.5 + textureH) /atlasH;
	
		vertexs[count * 16 + 6] = (textureX - 0.5 + textureW) / atlasW; // top right texcoord
		vertexs[count * 16 + 7] = (textureY - 0.5 + textureH) / atlasH;
		
		vertexs[count * 16 + 10] = (textureX - 0.5 + textureW) / atlasW; // bottom right texcoord
		vertexs[count * 16 + 11] = (textureY + 0.5) / atlasH; 
		
		vertexs[count * 16 + 14] = (textureX + 0.5) / atlasW; // bottom left texcoord
		vertexs[count * 16 + 15] = (textureY + 0.5) / atlasH;

		vertexs[count * 16 + 0] = intRect.x * 2.0 / SCREEN_WIDTH - 1.0;  // top left
		vertexs[count * 16 + 1] = intRect.y * 2.0 / SCREEN_HEIGHT - 1.0;

		vertexs[count * 16 + 4] = (intRect.x + intRect.w) * 2.0 / SCREEN_WIDTH - 1.0; //top right
		vertexs[count * 16 + 5] = intRect.y * 2.0 / SCREEN_HEIGHT - 1.0;


		vertexs[count * 16 + 8] = (intRect.x + intRect.w) * 2.0 / SCREEN_WIDTH - 1.0; //bottom right
		vertexs[count * 16 + 9] = (intRect.y + intRect.h) * 2.0 / SCREEN_HEIGHT - 1.0;

		vertexs[count * 16 + 12] = intRect.x * 2.0 / SCREEN_WIDTH - 1.0; //bottom left
		vertexs[count * 16 + 13] = (intRect.y + intRect.h) * 2.0 / SCREEN_HEIGHT - 1.0;


		glBindTexture(GL_TEXTURE_2D, txAtlasID);
		glBindVertexArray(objectShader.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, objectShader.VBO); 
		glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertexs, GL_DYNAMIC_DRAW);	

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectShader.EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderObjectSize[i] * 6 * sizeof(**elements), elements[i], GL_DYNAMIC_DRAW);
		GLint movement = glGetUniformLocation(objectShader.shaderProgram, "movement");
		glUniform3f(movement, (floor(viewport.x) * 2.0 + SCREEN_WIDTH) / SCREEN_WIDTH - 1.0, (floor(viewport.y) * 2.0 + SCREEN_HEIGHT) / SCREEN_HEIGHT - 1.0, 0);	
		glDrawArrays(GL_TRIANGLES, 0, 1 * 6);
		//glDrawElements(GL_TRIANGLES, renderObjectSize[i] * 6, GL_UNSIGNED_INT, 0);
	}*/


	//text rendering
 	renderText();

 	renderShapes();

	return 0;
}

