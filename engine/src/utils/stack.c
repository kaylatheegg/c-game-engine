#include "engine.h"

stack* createStack(size_t size, int type) {
	if (type > STACK_END) {
		type = STACK_FIFO;
	}
	stack* intStack = gmalloc(sizeof(*intStack));
	*intStack = (stack){createDynArray(size), type};
	return intStack;
}

//STACK_FILO time

void* popStack(stack* intStack) {
	if (intStack == NULL) {
		return NULL;
	}

	if (intStack->type == STACK_FIFO) {
		void* intElement = getElement(intStack->array, 0);
		void* tempAlloc = gmalloc(intStack->array->typeSize);
		memcpy(tempAlloc, intElement, intStack->array->typeSize);
		//its leaky, but i cant do anything about it! D:
		//BIG FIXME energy rn
		removeElement(intStack->array, 0);

		return tempAlloc;	
	}

	if (intStack->type == STACK_FILO) {
		void* intElement = getElement(intStack->array, intStack->array->arraySize - 1);
		void* tempAlloc = gmalloc(intStack->array->typeSize);
		memcpy(tempAlloc, intElement, intStack->array->typeSize);
		//its leaky, but i cant do anything about it! D:
		//BIG FIXME energy rn
		removeElement(intStack->array, intStack->array->arraySize - 1);
		return tempAlloc;
	}
	logtofile("If you're reading this, the stack done broke itself", SVR, "Stack");
	return NULL;
}

void pushStack(stack* intStack, void* element) {
	if (intStack == NULL) {
		return;
	}
	appendElement(intStack->array, element);
}

