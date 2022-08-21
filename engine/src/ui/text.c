#include "engine.h"

FT_Library ft;

void initText() {
	if (FT_Init_FreeType(&ft)) {
		logtofile("Failed to initialise freetype fonts!", SVR, "Fonts");
		crash();
	}

	if (FT_New_Face(ft, "engine/data/fonts/URWGothic-Book.otf", 0, &abyssinica)) {
		logtofile("Failed to load abyssinica font!", SVR, "Fonts");
		return;
	}
	FT_Set_Pixel_Sizes(abyssinica, 0, 128); 
	font = createDynArray(sizeof(character));

	//ripped from cpp help, this is q+d i just need text
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
  
	for (unsigned char c = 0; c < 255; c++) {
    	// load character glyph 
    	if (FT_Load_Char(abyssinica, c, FT_LOAD_RENDER)) {
        	logtofile("Failed to insert character!", ERR, "Fonts");
        	continue;
    	}
    	// generate texture
    	unsigned int texture;
    	glGenTextures(1, &texture);
    	glBindTexture(GL_TEXTURE_2D, texture);
    	glTexImage2D(
    	    GL_TEXTURE_2D,
   	     	0,
    	    GL_RED,
    	    abyssinica->glyph->bitmap.width,
     	    abyssinica->glyph->bitmap.rows,
     	    0,
     	    GL_RED,
     	    GL_UNSIGNED_BYTE,
     	    abyssinica->glyph->bitmap.buffer
    	);
    // set texture options
   		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // now store character for later use
    	character intChar = {
    	    texture, 
    	    VECCNT(abyssinica->glyph->bitmap.width, abyssinica->glyph->bitmap.rows),
    	    VECCNT(abyssinica->glyph->bitmap_left, abyssinica->glyph->bitmap_top),
     	    abyssinica->glyph->advance.x
    	};
    	appendElement(font, &intChar);
	}	

	/*FT_Done_Face(abyssinica);
	FT_Done_FreeType(ft);
	*/
	textShader = (program){.vertexPath = "engine/data/shaders/text.vs", .fragmentPath = "engine/data/shaders/text.fs"};

	loadShader(&textShader);

	glUseProgram(textShader.shaderProgram);

	glGenBuffers(1, &textShader.VBO); 
	glGenVertexArrays(1, &textShader.VAO); 
	glBindVertexArray(textShader.VAO);

	glBindBuffer(GL_ARRAY_BUFFER, textShader.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	GLint posAttrib = glGetAttribLocation(textShader.shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0); 
	glEnableVertexAttribArray(posAttrib);

	GLint texAttrib = glGetAttribLocation(textShader.shaderProgram, "texcoord");
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float))); //here
	glEnableVertexAttribArray(texAttrib);
	
	textStack = createStack(sizeof(textStackEntry), STACK_FIFO);
}


void drawText(char* text, float x, float y, float scale, RGBA colour) {
	pushStack(textStack, &(textStackEntry){strdup(text), x, y, scale/128, colour});
}

void renderText() {
	glUseProgram(textShader.shaderProgram);
	glBindVertexArray(textShader.VAO);	


	size_t stackSize = textStack->array->arraySize;
	for (size_t i = 0; i < stackSize; i++) {
		textStackEntry* textElement = (textStackEntry*)popStack(textStack); 
		RGBA colour = textElement->colour;
		float x = textElement->x;
		float y = textElement->y;
		//have to transform x and y into screenspace coordinates
		x = x * 2 - 800;
		y = y * 2 - 800;
		float scale = textElement->scale;
		char* text = textElement->text;
		glUniform3f(glGetUniformLocation(textShader.shaderProgram, "textColor"), colour.r, colour.g, colour.b);
		//intRect.x * 2.0 / SCREEN_WIDTH - 1.0;

	    // iterate through all characters
	    for (size_t j = 0; j < strlen(text); j++) {	
	    	if (text[j] == '\n') {
	    		y -= scale * 128; //this is hacky, but it works for now. the y advance should *really* be used instead
	    		x = textElement->x * 2 - 800;
	    		continue;
	    	}
	        character ch = *(character*)getElement(font, text[j]);	
	        float xpos = x + ch.bearing.x * scale;	

	        float ypos = y - (ch.size.y - ch.bearing.y) * scale;
	        float w = ch.size.x * scale;
	        float h = ch.size.y * scale;
	        // update VBO for each character
	        float charVertices[6][4] = {
	            { xpos,     ypos + h,   0.0f, 0.0f },            
	            { xpos,     ypos,       0.0f, 1.0f },
	            { xpos + w, ypos,       1.0f, 1.0f },	

	            { xpos,     ypos + h,   0.0f, 0.0f },
	            { xpos + w, ypos,       1.0f, 1.0f },
	            { xpos + w, ypos + h,   1.0f, 0.0f }           
	        };
	        // render glyph texture over quad
	        glBindTexture(GL_TEXTURE_2D, ch.txID);	

	        // update content of VBO memory
	        glBindBuffer(GL_ARRAY_BUFFER, textShader.VBO);
	        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(charVertices), charVertices); 
	        // render quad
	        glDrawArrays(GL_TRIANGLES, 0, 6);
	        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
	        x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
	    }
	}
}

