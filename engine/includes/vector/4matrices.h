#pragma once
#define FOURMATRICES_VECTOR_H

typedef struct {
	float values[4][4];
} matrix4;

matrix4 orthographicMatrix(float left, float right, float top, float bottom, float near, float far);
matrix4 perspectiveMatrix(float left, float right, float top, float bottom, float near, float far);
matrix4 mulMatrix(matrix4 a, matrix4 b);
matrix4 translationMatrix(float x, float y, float z);
matrix4 identity();
matrix4 transpose(matrix4 a);
vec vecMatrix(matrix4 a, vec b);