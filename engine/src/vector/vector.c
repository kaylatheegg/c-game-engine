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

vec vecProj(vec a, vec b) {
	double vecLen = vecLength(b);
	return vecScale(b ,(vecDot(a, b)/(vecLen*vecLen)));
}

vec vecRej(vec a, vec b) {
	return vecSub(a, vecProj(a, b));
}