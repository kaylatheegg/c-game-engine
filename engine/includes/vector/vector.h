#pragma once
#define VECTOR_VECTOR_H

typedef struct {
	double x;
	double y;
} vec;

typedef struct {
	double a,b;
	double c,d;
} mat;

#define VECCNT(x, y) (vec){x, y}
#define PI 3.1415926

vec vecAdd(vec a, vec b);
vec vecSub(vec a, vec b);
double vecLength(vec a);
double vecDot(vec a, vec b);
vec vecScale(vec a, double b);
vec vecProj(vec a, vec b);
vec vecRej(vec a, vec b);
vec vecMatProduct(vec a, mat b);
vec vecRotate(vec a, float b);
mat rotMatrix(float a);
vec vecNorm(vec a);
float vecAngle(vec a);