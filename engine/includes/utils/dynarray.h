#pragma once
#define UTILS_DYNARRAY_H

typedef struct {
	size_t typeSize;
	unsigned char* arrayData;
	size_t arraySize;
} dynArray;

dynArray* createDynArray(size_t typeSize);
void appendElement(dynArray* array, void* element); 
void removeElement(dynArray* array, size_t index);
void testArray();