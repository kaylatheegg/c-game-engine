#pragma once
#define UTILS_MEMORYHANDLER_H

#define gfree(ptr) \
	if (ptr == NULL) { \
		logtofile("Attempted to free a NULL pointer!", ERR, "Memory"); \
		crash(); \
	} else { \
		free(ptr); \
	} \
 	while(0) \

#define gzalloc(size) gcalloc(size, 1)



void* gmalloc(size_t size);
void* grealloc(void* ptr, size_t size);
void* gcalloc(size_t nitems, size_t size);

