#include "engine.h"

dynArray* createDynArray(size_t typeSize) {
	if (typeSize == 0) {
		logtofile("typeSize == 0, returning!", WRN, "Array");
		return NULL;
	}
	
	dynArray* intArray = gmalloc(sizeof(*intArray));

	intArray->typeSize = typeSize;
	intArray->arraySize = 0;

	intArray->arrayData = gmalloc(typeSize);
	return intArray;
}

void appendElement(dynArray* array, void* element) {
	array->arraySize++;
	array->arrayData = grealloc(array->arrayData, array->typeSize*array->arraySize);
	memcpy(&array->arrayData[array->typeSize * (array->arraySize - 1)], element, array->typeSize);
}

void removeElement(dynArray* array, size_t index) {
	if (index > array->arraySize) {
		return;
	}
	memcpy(&array->arrayData[array->typeSize * index], &array->arrayData[array->typeSize * (index + 1)], array->typeSize * (array->arraySize - index));
	array->arraySize--;
	array->arrayData = grealloc(array->arrayData, array->typeSize*array->arraySize);
}

void* getElement(dynArray* array, size_t index) {
	return (void*)&array->arrayData[array->typeSize * index]; 
}

void printElements(dynArray* array) {
	for (size_t i = 0; i < array->arraySize; i++) {
		printf("%d\n", *(int*)getElement(array, i));
	}
}

void testArray() {
	dynArray* intArray = createDynArray(sizeof(int));
	//printElements(intArray);
	for (int i = 0; i < 10; i++) {
		appendElement(intArray, &i);
	}
	removeElement(intArray, 5);
	printElements(intArray);
}