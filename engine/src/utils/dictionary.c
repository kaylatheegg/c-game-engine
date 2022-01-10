#include "engine.h"

/*
typedef struct dict_t {
	dynArray* key;
	dynArray* value;
} dict_list;

typedef dict_list* dictionary;*/

dictionary createDictionary() {
	dictionary intDict = gmalloc(sizeof(dict_list));
	intDict->key = createDynArray(sizeof(char*));
	intDict->value = createDynArray(sizeof(void*));
	/*head->key = strdup("DICTIONARY--Head!--DICTIONARY");
	head->value = NULL;
	head->next = NULL;
	head->tail = head;*/

	return intDict;
}

/*dictionary findTail(dictionary head) {
	return head->tail;
}*/

void printDictionary(dictionary intDict) {
	logtofile("printing dict:", INF, "Dictionary");

	for (size_t i = 0; i < intDict->key->arraySize; i++) {
		if (getElement(intDict->key, i) == NULL) {
			logtofile("key: NULL", INF, "Dictionary");
		} else {
			char buffer[512];
			sprintf(buffer, "key: %.256s", *(char**)getElement(intDict->key, i));
			logtofile(buffer, INF, "Dictionary");
		}
	}
	logtofile("ending dictionary", INF, "Dictionary");
}

void addToDictionary(dictionary intDict, const char* key, void* value) {
	/*dictionary tail = findTail(head);
	if (tail == NULL) {
		logtofile("findTail() returned NULL on addToDictionary(), returning prematurely. please be advised!", ERR, "Dictionary");
		char buffer[512];
		sprintf(buffer, "key: %.256s", key);
		logtofile(buffer, ERR, "Dictionary");
		return;
	}*/



	/*tail->next->key = gmalloc(sizeof(char) * strlen(intKey) + 1);
	strcpy((char*)tail->next->key, (char*)intKey);
	tail->next->value = value;
	tail->next->next = NULL;
	head->tail = tail->next;*/

	const char* intKey = key;

	if (intKey == NULL) {
		intKey = strdup("KeyWasNull");
	} else {
		intKey = strdup(key);
	}

	appendElement(intDict->key, &intKey);
	appendElement(intDict->value, &value);
}

size_t findKey(dictionary intDict, const char* key) {
	for (size_t i = 0; i < intDict->key->arraySize; i++) {	
		if (strcmp(*(char**)getElement(intDict->key, i), key) == 0) {
			return i;
		}
	}

	return NOVALUE;
}

void freeDictionary(dictionary intDict) {
	deleteArray(intDict->value);
	deleteArray(intDict->key);
	gfree(intDict);
}



/*dictionary findPrevKey(dictionary head, const char* key) {
	dictionary current = head;
	dictionary prev = NULL;

	while (strcmp(current->key, key) != 0 && current != NULL) {
		prev = current;
		current = current->next;
	}
	return prev;
}*/

void removeKey(dictionary intDict, const char* key) {
	size_t keyIndex = findKey(intDict, key);
	if (keyIndex == NOVALUE) {
		logtofile("findKey() returned null, key not found in dict.", WRN, "Dictionary");
		return;
	}
	gfree(*(char**)getElement(intDict->key, keyIndex));
	removeElement(intDict->key, keyIndex);
	removeElement(intDict->value, keyIndex);
}

void updateValue(dictionary intDict, const char* key, void* value) {
	size_t keyIndex = findKey(intDict, key);
	if (keyIndex == NOVALUE) {
		logtofile("findKey() returned null, key not found in dict.", WRN, "Dictionary");
		return;
	}

	updateElement(intDict->value, keyIndex, &value);
}