#pragma once
#define UTILS_MEMORYHANDLER_H

void gfree(void* ptr);
void* gmalloc(size_t size);
void* grealloc(void* ptr, size_t size);