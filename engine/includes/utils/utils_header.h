#pragma once
#define UTILS_HEADER_H

//below are some helpful macros!
#define itoa(int, buffer) sprintf(buffer, "%d", int)

#define UNUSED(unused) (void)unused

#define map(x, in_min, in_max, out_min, out_max) ((x) - (in_min)) * ((out_max) - (out_min)) / ((in_max) - (in_min)) + (out_min)

#include "dictionary.h"
#include "ansicolours.h"
#include "memoryhandler.h"
#include "dynarray.h"