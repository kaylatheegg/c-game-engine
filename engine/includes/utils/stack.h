#pragma once
#define UTILS_STACK_H

typedef struct {
	dynArray* array;
	int type;
} stack;

enum stacks {
	STACK_FIFO = 0,
	STACK_FILO = 1,
	STACK_END
};

stack* createStack(size_t size, int type);
void* popStack(stack* intStack);
void pushStack(stack* intStack, void* element);

