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
void insertElement(dynArray* array, void* element, size_t index);
void* getElement(dynArray* array, size_t index);
void* popElement(dynArray* array);
void clearArray(dynArray* array);
void updateElement(dynArray* array, size_t index, void* value);
void deleteArray(dynArray* array);