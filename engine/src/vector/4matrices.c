#include "engine.h"
//generating an orthographic projection allows an easy way to take worldspace coordinates to gl coordinates
//while maintaining scale
//these calculations SADLY create a transposed matrix, so they need to be correctly transformed to fix this

matrix4 transpose(matrix4 a) {
	matrix4 intMatrix;
	float values[4][4] = {0};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			values[i][j] = a.values[j][i];
		}
	}
	memcpy(intMatrix.values, values, sizeof(float)*16);
	return intMatrix;
}


matrix4 orthographicMatrix(float left, float right, float top, float bottom, float near, float far) {
	//the following formulae are copied from https://en.wikipedia.org/wiki/Orthographic_projection
	matrix4 intMatrix;
	float values[4][4] = {{2.0 / (right - left), 0, 				   0, 					-(right + left) / (right - left)},
						  {0, 				   	 2.0 / (top - bottom), 0, 					-(top + bottom) / (top - bottom)},
						  {0, 				     0, 				   -2.0 / (far - near), -(far + near) / (far - near) 	},
						  {0, 				     0, 				   0, 					1 								}};
	memcpy(intMatrix.values, values, sizeof(float)*16);
	intMatrix = transpose(intMatrix);
	return intMatrix;
}

matrix4 perspectiveMatrix(float left, float right, float top, float bottom, float near, float far) {
	//the following formulae are copied from https://en.wikipedia.org/wiki/Orthographic_projection
	matrix4 intMatrix;
	float values[4][4] = {{2.0 * near / (right - left), 0, 				             (right + left) / (right - left), 0                               },
						  {0, 				   	 		2.0 * near / (top - bottom), (top + bottom) / (top - bottom), 0               			      },
						  {0, 				     		0, 				   			 -(far + near) / (far - near),    -2.0 * far * near / (far - near)},
						  {0, 				    		0, 				  			 -1, 					0 						                  }};
	memcpy(intMatrix.values, values, sizeof(float)*16);
	intMatrix = transpose(intMatrix);
	return intMatrix;
}

matrix4 translationMatrix(float x, float y, float z) {
	matrix4 intMatrix;
	float values[4][4] = {{1, 0, 0, x},
						  {0, 1, 0, y},
						  {0, 0, 1, z},
						  {0, 0, 0, 1}};
	memcpy(intMatrix.values, values, sizeof(float)*16);
	intMatrix = transpose(intMatrix);
	return intMatrix;
	//by applying this matrix to a vec4, you will see that this occurs:
	//vec4 -> vec4 + VECCNT(x, y, z);
}

matrix4 mulMatrix(matrix4 a, matrix4 b) {
	//dear god this is gonna suck
	float values[4][4] = {0};
	matrix4 intMatrix;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			//we basically do the dot product on row a, and column b for EVERY value.
			values[i][j] = a.values[i][0] * b.values[0][j] +
						   a.values[i][1] * b.values[1][j] + 
						   a.values[i][2] * b.values[2][j] + 
						   a.values[i][3] * b.values[3][j]; 
		}
	}
	memcpy(intMatrix.values, values, sizeof(float)*16);
	return intMatrix;
}

vec vecMatrix(matrix4 a, vec b) {
	vec intVec = (vec){0, 0};
	intVec.x = a.values[0][0] * b.x + a.values[0][1] * b.y + a.values[0][3] * 1;
	intVec.x = a.values[1][0] * b.x + a.values[1][1] * b.y + a.values[1][3] * 1;
	return intVec;
}

matrix4 identity() {
	float values[4][4] = {0};
	matrix4 intMatrix;
	for (int i = 0; i < 4; i++) {
		values[i][i] = 1;
	}
	memcpy(intMatrix.values, values, sizeof(float)*16);
	return intMatrix;
}