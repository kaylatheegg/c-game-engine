#pragma once
#define VECTOR_VECTOR_H

typedef struct {
	double x;
	double y;
} vec;

#define VECCNT(x, y) (vec){x, y}

vec vecAdd(vec a, vec b);
vec vecSub(vec a, vec b);
double vecLength(vec a);
double vecDot(vec a, vec b);
vec vecScale(vec a, double b);
vec vecProj(vec a, vec b);