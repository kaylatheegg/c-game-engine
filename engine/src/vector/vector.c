#include "engine.h"

vec vecAdd(vec a, vec b) {
	return VECCNT(a.x + b.x, a.y + b.y);
}

vec vecSub(vec a, vec b) {
	return VECCNT(a.x - b.x, a.y - b.y);
}

double vecLength(vec a) {
	return sqrt(a.x * a.x + a.y * a.y);
}

double vecDot(vec a, vec b) {
	return a.x * b.x + a.y * b.y;
}

vec vecScale(vec a, double b) {
	return VECCNT(a.x * b, a.y * b);
}

vec vecNorm(vec a) {
	return vecScale(a, 1./vecLength(a));
}

vec vecProj(vec a, vec b) {
	double vecLen = vecLength(b);

	return vecScale(b,(vecDot(a, b)/(vecLen*vecLen)));
}

vec vecRej(vec a, vec b) {
	return vecSub(a, vecProj(a, b));
}

float vecAngle(vec a) {
	return (atan2(-a.y,a.x)) * (180/PI);
}

vec vecMatProduct(vec a, mat b) {
	return VECCNT(b.a * a.x + b.b * a.y,
				  b.c * a.x + b.d * a.y);
}

mat rotMatrix(float a) {
	double radians = (PI/180.) * a;
	return (mat){cos(radians), -sin(radians),
				 sin(radians), cos(radians)};
}

vec vecRotate(vec a, float b) {
	return vecMatProduct(a, rotMatrix(b));
}
