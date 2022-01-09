#pragma once
#define UTILS_DICT_H

typedef struct dict_t {
	dynArray* key;
	dynArray* value;
} dict_list;

typedef dict_list* dictionary;

dictionary createDictionary();
//dictionary findTail(dictionary hwead);
void printDictionary(dictionary head);
void addToDictionary(dictionary head, const char* key, void* value);
size_t findKey(dictionary intDict, const char* key);
void freeDictionary(dictionary head);
void removeKey(dictionary head, const char* key);
dictionary findPrevKey(dictionary head, const char* key);
dictionary findTail(dictionary head);