#pragma once
#define UTILS_MEMORYHANDLER_H

#define gfree(ptr) \
	if (ptr == NULL) { \
		logtofile("Attempted to free a NULL pointer!", ERR, "Memory"); \
	} else { \
		free(ptr); \
	} \
 	while(0) \


void* gmalloc(size_t size);
void* grealloc(void* ptr, size_t size);