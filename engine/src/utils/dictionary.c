#include "engine.h"

/*
typedef struct dict_t {
	char* key;
	void* value;
	struct dict_t* next;
} dict_list;

typedef dict_list* dictionary;*/

dictionary createDictionary() {
	dictionary head = gmalloc(sizeof(dict_list));
	head->key = strdup("DICTIONARY--Head!--DICTIONARY");
	head->value = NULL;
	head->next = NULL;

	return head;
}

dictionary findTail(dictionary head) {
	dictionary current = head;
	dictionary prev = NULL;
	while (current != NULL) {
		prev = current;
		current = current->next;
	}
	return prev;
}

void printDictionary(dictionary head) {
	dictionary current = head;
	logtofile("printing dict:", INF, "Dictionary");

	while (current != NULL) {
		if (current->key == NULL) {
			logtofile("key: NULL", INF, "Dictionary");
		} else {
			char buffer[512];
			sprintf(buffer, "key: %.256s", current->key);
			logtofile(buffer, INF, "Dictionary");
		}
		current = current->next;
	}
	logtofile("ending dictionary", INF, "Dictionary");
}

void addToDictionary(dictionary head, char* key, void* value) {
	dictionary tail = findTail(head);
	if (tail == NULL) {
		logtofile("findTail() returned NULL on addToDictionary(), returning prematurely. please be advised!", ERR, "Dictionary");
		return;
	}


	tail->next = gmalloc(sizeof(dict_list));

	char* intKey = key;

	if (intKey == NULL) {
		intKey = "KeyWasNull";
	}

	tail->next->key = gmalloc(sizeof(char) * strlen(intKey) + 1);
	strcpy(tail->next->key, intKey);
	tail->next->value = value;
	tail->next->next = NULL;
}

dictionary findKey(dictionary head, char* key) {
	dictionary current = head;

	while (current != NULL && current->key != NULL) {
		//printf("key: %s\n", current->key);
		if (strcmp(current->key, key) == 0) {
			return current;
		}

		current = current->next;
	}

	return NULL;
}

void freeDictionary(dictionary head) {
	dictionary temp;

  	while (head != NULL) {
	    temp = head;
        head = head->next;
        gfree(temp->key);

        gfree(temp);
    }

}

dictionary findPrevKey(dictionary head, char* key) {
	dictionary current = head;
	dictionary prev = NULL;

	while (strcmp(current->key, key) != 0 && current != NULL) {
		prev = current;
		current = current->next;
	}
	return prev;
}

void removeKey(dictionary head, char* key) {
	dictionary keyIndex = findKey(head, key);
	if (keyIndex == NULL) {
		logtofile("findKey() returned null, key not found in dict.", WRN, "Dictionary");
		return;
	}
	
	dictionary prevKeyIndex = findPrevKey(head, keyIndex->key);
	if (prevKeyIndex == NULL) {
		logtofile("findPrevKey() returned null, what the fuck. this is some UB long titty no nipple having ass bitch. this call should not have fucking failed, please like, message the dev", ERR, "Dictionary");
		return;
	}

	prevKeyIndex->next = keyIndex->next;

	gfree(keyIndex->key);
	//gfree(keyIndex);
}

void updateValue(dictionary head, char* key, void* value) {
	dictionary keyIndex = findKey(head, key);
	if (keyIndex == NULL) {
		logtofile("findKey() returned null, key not found in dict.", WRN, "Dictionary");
		return;
	}

	keyIndex->value = value;
}