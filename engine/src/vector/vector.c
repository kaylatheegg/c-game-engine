#include "engine.h"

vec vecAdd(vec a, vec b) {
	return VECCNT(a.x + b.x, a.y + b.y);
}

vec vecSub(vec a, vec b) {
	return VECCNT(a.x - b.x, a.y - b.y);
}

double vecLength(vec a) {
	return isnan(sqrt(a.x * a.x + a.y * a.y)) ? 0.0 : sqrt(a.x * a.x + a.y * a.y);
}

double vecDot(vec a, vec b) {
	return a.x * b.x + a.y * b.y;
}

vec vecScale(vec a, double b) {
	return VECCNT(a.x * b, a.y * b);
}

vec vecNorm(vec a) {
	if (vecLength(a) == 0) return VECCNT(0,0);
	return vecScale(a, 1./vecLength(a));
}

vec vecProj(vec a, vec b) {
	double vecLen = vecLength(b);

	return vecScale(b,(vecDot(a, b)/(vecLen*vecLen)));
}

vec vecRej(vec a, vec b) {
	return vecSub(a, vecProj(a, b));
}

double vecAngle(vec a) {
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

vec vecRotateAroundOrigin(vec a, vec origin, float b) {
	return vecAdd(vecRotate(vecSub(a, origin), b), origin);
}

vec vecInvert(vec a) {
	return VECCNT(1/a.x, 1/a.y);
}