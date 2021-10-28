#pragma once
#define UTILS_DICT_H

typedef struct dict_t {
	const char* key;
	void* value;
	struct dict_t* next;
} dict_list;

typedef dict_list* dictionary;

dictionary createDictionary();
//dictionary findTail(dictionary hwead);
void printDictionary(dictionary head);
void addToDictionary(dictionary head, const char* key, void* value);
dictionary findKey(dictionary head, const char* key);
void freeDictionary(dictionary head);
void removeKey(dictionary head, const char* key);
dictionary findPrevKey(dictionary head, const char* key);
dictionary findTail(dictionary head);