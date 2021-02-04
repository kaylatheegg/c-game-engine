#pragma once
#define UTILS_DICT_H

typedef struct dict_t {
	char* key;
	void* value;
	struct dict_t* next;
} dict_list;

typedef dict_list* dictionary;

dictionary createDictionary();
//dictionary findTail(dictionary hwead);
void printDictionary(dictionary head);
void addToDictionary(dictionary head, char* key, void* value);
dictionary findKey(dictionary head, char* key);
void freeDictionary(dictionary head);
void removeKey(dictionary head, char* key);