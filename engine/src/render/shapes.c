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

	GLint circlePosAttrib = glGetAttribLocation(circleShader.shaderProgram, "position");
	glVertexAttribPointer(circlePosAttrib, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0); 
	glEnableVertexAttribArray(circlePosAttrib);



	lineShader = (program){.vertexPath = "engine/data/shaders/line.vs", .fragmentPath = "engine/data/shaders/line.fs"};
	loadShader(&lineShader);

	glUseProgram(lineShader.shaderProgram);

	glGenVertexArrays(1, &lineShader.VAO); 
	glBindVertexArray(lineShader.VAO);

	glGenBuffers(1, &lineShader.VBO); 
	glBindBuffer(GL_ARRAY_BUFFER, lineShader.VBO); 

	GLint linePosAttrib = glGetAttribLocation(lineShader.shaderProgram, "position");
	glVertexAttribPointer(linePosAttrib, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0); 
	glEnableVertexAttribArray(linePosAttrib);

	glEnable(GL_LINE_SMOOTH);

}

int drawCircle(vec pos, float radius, RGBA colour) {
	pushStack(shapeStack, &(shape){.type = SHAPE_CIRCLE,
								   .start = pos,
								   .end = VECCNT(0,0),
								   .radius = radius*2,
								   .colour = colour});
	return 0;
}

int drawLine(vec start, vec end, RGBA colour, float thickness) {
	pushStack(shapeStack, &(shape){.type = SHAPE_LINE,
								   .start = start,
								   .end = end,
								   .radius = thickness,
								   .colour = colour});

	//we should push two circles onto the stack here for a "rounded" line edge,
	//but this isnt too much of an issue tbh
	return 0;
}

int renderShapes() {
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
			glBindBuffer(GL_ARRAY_BUFFER, circleShader.VBO);

			//this whole fucking thing is broken and should be rewritten CORRECTLY at some point
			//the vertex generation is all fucked, the position of the circle is all fucked
			//but fuck it, it works

			//make the vertices needed to draw the circle
			glUniform3f(glGetUniformLocation(circleShader.shaderProgram, "circleColour"), intShape->colour.r, intShape->colour.g, intShape->colour.b);
			glUniform2f(glGetUniformLocation(circleShader.shaderProgram, "circleCenter"), SCREEN_WIDTH/2 + (intShape->start.x - SCREEN_WIDTH/2)/2, SCREEN_WIDTH/2 + (intShape->start.y - SCREEN_WIDTH/2)/2);
			glUniform1f(glGetUniformLocation(circleShader.shaderProgram, "radius"), intShape->radius/2);



			float shapeVertices[6][4] = {
	            { intShape->start.x - intShape->radius - SCREEN_WIDTH/2, intShape->start.y + intShape->radius - SCREEN_HEIGHT/2,   0.0f, 0.0f },            
	            { intShape->start.x - intShape->radius - SCREEN_WIDTH/2, intShape->start.y - intShape->radius - SCREEN_HEIGHT/2,   0.0f, 1.0f },
	            { intShape->start.x + intShape->radius - SCREEN_WIDTH/2, intShape->start.y - intShape->radius - SCREEN_HEIGHT/2,   1.0f, 1.0f },	

	            { intShape->start.x - intShape->radius - SCREEN_WIDTH/2, intShape->start.y + intShape->radius - SCREEN_HEIGHT/2,   0.0f, 0.0f },
	            { intShape->start.x + intShape->radius - SCREEN_WIDTH/2, intShape->start.y - intShape->radius - SCREEN_HEIGHT/2,   1.0f, 1.0f },
	            { intShape->start.x + intShape->radius - SCREEN_WIDTH/2, intShape->start.y + intShape->radius - SCREEN_HEIGHT/2,   1.0f, 0.0f }           
	        };

	        /*float charVertices[6][4] = {
	            { xpos,     ypos + h,   0.0f, 0.0f },            
	            { xpos,     ypos,       0.0f, 1.0f },
	            { xpos + w, ypos,       1.0f, 1.0f },	

	            { xpos,     ypos + h,   0.0f, 0.0f },
	            { xpos + w, ypos,       1.0f, 1.0f },
	            { xpos + w, ypos + h,   1.0f, 0.0f }           
	        };*/

	        glBufferData(GL_ARRAY_BUFFER, sizeof(shapeVertices), shapeVertices, GL_DYNAMIC_DRAW); 
	        // render quad
	        glDrawArrays(GL_TRIANGLES, 0, 6);
		
		}

		if (intShape->type == SHAPE_LINE) {
			glUseProgram(lineShader.shaderProgram);
			glBindVertexArray(lineShader.VAO);	
			glBindBuffer(GL_ARRAY_BUFFER, lineShader.VBO);

			glUniform3f(glGetUniformLocation(lineShader.shaderProgram, "lineColour"), intShape->colour.r, intShape->colour.g, intShape->colour.b);
			glLineWidth(intShape->radius);

			float lineVertices[2][4] = {
	            { intShape->start.x - SCREEN_WIDTH/2, intShape->start.y - SCREEN_HEIGHT/2,   0.0f, 0.0f },            
	            { intShape->end.x   - SCREEN_WIDTH/2, intShape->end.y   - SCREEN_HEIGHT/2,   1.0f, 1.0f }        
			};

			glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_DYNAMIC_DRAW);
			glDrawArrays(GL_LINES, 0, 2);
		}

		if (intShape->type > SHAPE_END) {
			popStack(shapeStack);
		}
	}

	return 0;
}