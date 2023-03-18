#pragma once
#define UTILS_HEADER_H

//below are some helpful macros!
#define itoa(int, buffer) sprintf(buffer, "%d", int)

#define UNUSED(unused) (void)unused

#define map(x, in_min, in_max, out_min, out_max) ((x) - (in_min)) * ((out_max) - (out_min)) / ((in_max) - (in_min)) + (out_min)

#define max(a, b) (((a) < (b)) ? (a) : (b))
#define min(a, b) (((a) > (b)) ? (a) : (b))

#define CONST_E  2.718281828
#define CONST_PI 3.141592635

#include "dynarray.h"
#include "dictionary.h"
#include "ansicolours.h"
#include "memoryhandler.h"
#include "perlin.h"
#include "stack.h"
#include "random.h"
#include "graph.h"
