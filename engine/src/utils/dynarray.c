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
	if (array == NULL) {
		return;
	}

	array->arraySize++;
	array->arrayData = grealloc(array->arrayData, array->typeSize*array->arraySize);
	memcpy(&array->arrayData[array->typeSize * (array->arraySize - 1)], element, array->typeSize);
}

void updateElement(dynArray* array, size_t index, void* value) {
	if (array == NULL) {
		return;
	}	

	if (index > array->arraySize) {
		return;
	}
	memcpy(&array->arrayData[array->typeSize * index], value, array->typeSize);
}

void removeElement(dynArray* array, size_t index) {
	if (array == NULL) {
		return;
	}

	if (index > array->arraySize) {
		return;
	}
	memcpy(&array->arrayData[array->typeSize * index], &array->arrayData[array->typeSize * (index + 1)], array->typeSize * (array->arraySize - index - 1));
	array->arraySize--;
	array->arrayData = grealloc(array->arrayData, array->typeSize*array->arraySize);
}

void insertElement(dynArray* array, void* element, size_t index) {
	if (array == NULL) {
		return;
	}

	if (index > array->arraySize) {
		return;
	}
	array->arraySize++;
	array->arrayData = grealloc(array->arrayData, array->typeSize*array->arraySize);
	//copy elements from index onwards forward by 1 size, then insert it
	memcpy(&array->arrayData[array->typeSize * (index + 1)], &array->arrayData[array->typeSize * (index)], array->typeSize * (array->arraySize - index - 1));
	memcpy(&array->arrayData[array->typeSize * index], element, array->typeSize);
}

void* getElement(dynArray* array, size_t index) {
	if (array == NULL) {
		return NULL;
	}

	if (index > array->arraySize) {
		return NULL;
	}

	return (void*)&array->arrayData[array->typeSize * index]; 
}

void* popElement(dynArray* array) {
	if (array == NULL) {
		return NULL;
	}

	void* intElement = getElement(array, array->arraySize - 1);
	void* tempAlloc = malloc(array->typeSize);
	memcpy(tempAlloc, intElement, array->typeSize);
	//its leaky, but i cant do anything about it! D:
	//BIG FIXME energy rn
	removeElement(array, array->arraySize - 1);

	return tempAlloc;
}

void printElements(dynArray* array) {
	if (array == NULL) {
		return;
	}

	if (array->arraySize == 0) {
		logtofile("attempted to print empty array!", INF, "DynArray");
		return;
	}

	for (size_t i = 0; i < array->arraySize; i++) {
		printf("%d\n", *(int*)getElement(array, i));
	}
	printf("\n");
}

void clearArray(dynArray* array) {
	if (array == NULL) {
		return;
	}

	for (size_t i = 0; i < array->arraySize; i++) {
		removeElement(array, 0);
	}
}

void testArray() {

	dynArray* intArray = createDynArray(sizeof(int));
	//printElements(intArray);
	for (int i = 0; i < 15; i++) {
		appendElement(intArray, &i);
	}
	printElements(intArray);
	for (int i = 0; i < 1500; i++) {
		int randomValue = rand() % 16;
		switch (rand() % 3) {
			case 0:

				appendElement(intArray, &randomValue);
				break;
			case 1:
				removeElement(intArray, i);
				break;
			case 2:
				insertElement(intArray, &i, rand() % intArray->arraySize);
				break;
		}
	}
	printElements(intArray);
	clearArray(intArray);
	printElements(intArray);
}
