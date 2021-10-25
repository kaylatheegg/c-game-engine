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
		logtofile("remalloc returned NULL on call, crashing!", SVR, "Memory");
		crash();
	}
	return intPtr;
}