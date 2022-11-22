#pragma once
#define UTILS_STACK_H

enum stacks {
	STACK_FIFO = 0,
	STACK_FILO = 1,
	STACK_END
};

typedef struct {
	dynArray* array;
	enum stacks type;
} stack;

stack* createStack(size_t size, enum stacks type);
void* popStack(stack* intStack);
void pushStack(stack* intStack, void* element);

