#include "engine.h"

//memory handling functions

void* gmalloc(size_t size) {
	void* intPtr = malloc(size);
	if (intPtr == NULL) {
		logtofile("malloc returned NULL on call, crashing!", SVR, "Memory");
		crash();
	}
	return intPtr;
}

void* grealloc(void* ptr, size_t size) {
	if (ptr == NULL) {
		logtofile("Attempted to reallocate a NULL pointer, consider using malloc.", ERR, "Memory");
		return NULL;
	}
	void* intPtr = realloc(ptr, size);
	if (intPtr == NULL) {
		logtofile("realloc returned NULL on call, returning previous data!", ERR, "Memory");
#ifdef DEBUG
		crash();
#endif
		return ptr;
	}
	return intPtr;
}

void* gcalloc(size_t nitems, size_t size) {
	if (nitems == 0 || size == 0) {
		logtofile("Attempted to create a calloc with no size or items!", ERR, "Memory");
#ifdef DEBUG
		crash();
#endif
		return NULL;
	}
	void* intPtr = calloc(nitems, size);
	if (intPtr == NULL) {
		logtofile("calloc returned NULL on call, crashing!", ERR, "Memory");
		crash();
	}

	return intPtr;
}