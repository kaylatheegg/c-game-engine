#include "engine.h"

//shape drawing stack

void initShapes() {
	shapeStack = createStack(sizeof(shape), STACK_FIFO);
	//construct shape shader programs
	circleShader = (program){.vertexPath = "engine/data/shaders/circle.vs", .fragmentPath = "engine/data/shaders/circle.fs"};
	loadShader(&circleShader);
	//printf("%d", objectShader->shaderProgram);
	
	glUseProgram(circleShader.shaderProgram);

	glGenVertexArrays(1, &circleShader.VAO); 
	glBindVertexArray(circleShader.VAO);

	glGenBuffers(1, &circleShader.VBO); 
	glBindBuffer(GL_ARRAY_BUFFER, circleShader.VBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &circleShader.EBO);

	GLint posAttrib = glGetAttribLocation(circleShader.shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0); 
	glEnableVertexAttribArray(posAttrib);
}

int drawCircle(vec pos, float radius, RGBA colour) {
	pushStack(shapeStack, &(shape){.type = SHAPE_CIRCLE,
								   .start = pos,
								   .end = VECCNT(0,0),
								   .radius = radius,
								   .colour = colour});
	return 0;
}

int renderShapes() {
	drawCircle(VECCNT(200, 200), 80., (RGBA){.rgba = 0xFFBCF8FF});
	drawCircle(VECCNT(300, 200), 80., (RGBA){.rgba = 0xFFBCF8FF});
	drawCircle(VECCNT(400, 200), 80., (RGBA){.rgba = 0xFFBCF8FF});
	drawCircle(VECCNT(500, 200), 80., (RGBA){.rgba = 0xFFBCF8FF});

	size_t stackSize = shapeStack->array->arraySize;
	for (size_t i = 0; i < stackSize; i++) {
		shape* intShape = (shape*)popStack(shapeStack);
		if (intShape == NULL) {
			return -1;
		}

		if (intShape->type == SHAPE_CIRCLE) {
			//handle circle drawing
			glUseProgram(circleShader.shaderProgram);
			glBindVertexArray(circleShader.VAO);	
			//make the vertices needed to draw the circle
			glUniform3f(glGetUniformLocation(circleShader.shaderProgram, "circleColour"), intShape->colour.r, intShape->colour.g, intShape->colour.b);
			glUniform2f(glGetUniformLocation(circleShader.shaderProgram, "circleCenter"), intShape->start.x, intShape->start.y);


			float shapeVertices[6][4] = {
	            { intShape->start.x - intShape->radius, intShape->start.y + intShape->radius,   0.0f, 0.0f },            
	            { intShape->start.x - intShape->radius, intShape->start.y - intShape->radius,   0.0f, 1.0f },
	            { intShape->start.x + intShape->radius, intShape->start.y - intShape->radius,   1.0f, 1.0f },	

	            { intShape->start.x - intShape->radius, intShape->start.y + intShape->radius,   0.0f, 0.0f },
	            { intShape->start.x + intShape->radius, intShape->start.y - intShape->radius,   1.0f, 1.0f },
	            { intShape->start.x + intShape->radius, intShape->start.y + intShape->radius,   1.0f, 0.0f }           
	        };

	        /*float charVertices[6][4] = {
	            { xpos,     ypos + h,   0.0f, 0.0f },            
	            { xpos,     ypos,       0.0f, 1.0f },
	            { xpos + w, ypos,       1.0f, 1.0f },	

	            { xpos,     ypos + h,   0.0f, 0.0f },
	            { xpos + w, ypos,       1.0f, 1.0f },
	            { xpos + w, ypos + h,   1.0f, 0.0f }           
	        };*/


	        glBindBuffer(GL_ARRAY_BUFFER, circleShader.VBO);
	        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(shapeVertices), shapeVertices); 
	        // render quad
	        glDrawArrays(GL_TRIANGLES, 0, 6);
		
	        //vertices are on gpu, the fragment shader isnt working for some reason
		}
	}

	return 0;
}