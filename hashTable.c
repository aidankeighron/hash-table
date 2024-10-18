#define PY_SSIZE_T_CLEAN
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "hashTable.h"

HashTable initHashTable(int size) {
    if (size <= 0) {
        printf("Invalid Size");
        return NULL;
    }
    HashTable hashTable = (HashTable)calloc(size+1, sizeof(HashTableElement*));
    for (int i = 0; i < size; ++i) {
        HashTableElement* element = (HashTableElement*)calloc(1, sizeof(HashTableElement));
        element->key = NULL;
        element->value = 0;
        element->deleted = false;
        hashTable[i] = element;
    }

    hashTable[size] = NULL;

    return hashTable;
}

void _print(HashTable hashTable, bool printNull) {
    printf("{");
    for (; *hashTable != NULL; ++hashTable) {
        if ((*hashTable)->key != NULL || printNull) {
            printf("%s: %d, ", (*hashTable)->key, (*hashTable)->value);
        }
    }
    printf("}\n");
}

long hash(char* key) {
    long hash = 0;
    int i = 0;
    for (; *key; ++key) {
        hash = 18 * hash + *key + i++;
    }

    return hash % hashMapSize;
}

void grow() {
    hashMapSize *= 2;
    capacity = 0;
    HashTable newHashTable = initHashTable(hashMapSize);

    HashTable start = hashTable;
    for (; (*hashTable) != NULL; ++(hashTable)) {
        if ((*hashTable)->key != NULL) {
            _set(newHashTable, (*hashTable)->key, (*hashTable)->value);
            free((*hashTable)->key);
            free(*hashTable);
        }
    }

    free(start);
    hashTable = newHashTable;
}

void _set(HashTable hashTable, char* key, int value) {
    int index = hash(key);
    printf("%d\n", index);
    while (hashTable[index]->key != NULL && *hashTable[index]->key != *key) {
        index += 1;
        index %= hashMapSize;
    }

    if (hashTable[index]->key == NULL) {
        hashTable[index]->key = (char *)calloc(100, sizeof(char));
        capacity += 1;
    }
    strncpy(hashTable[index]->key, key, 100);
    hashTable[index]->value = value;

    if (capacity > hashMapSize/2) {
        grow();
    }
}

int _get(HashTable hashTable, char* key) {
    int index = hash(key);

    while ((hashTable[index]->key == NULL && hashTable[index]->deleted) || 
        (hashTable[index]->key != NULL && *hashTable[index]->key != *key)) {
        index += 1;
        index %= hashMapSize;
    }
    if (hashTable[index]->key == NULL) {
        // printf("Not found\n");
        return -1;
    }

    return hashTable[index]->value;
}

void _delete(HashTable hashTable, char* key) {
    int index = hash(key);
    while ((hashTable[index]->key == NULL && hashTable[index]->deleted) || 
        (hashTable[index]->key != NULL && *hashTable[index]->key != *key)) {
        index += 1;
        index %= hashMapSize;
    }
    if (hashTable[index]->key == NULL) {
        // printf("Not found\n");
        return;
    }

    capacity -= 1;
    free(hashTable[index]->key);
    hashTable[index]->key = NULL;
    hashTable[index]->value = 0;
    hashTable[index]->deleted = true;
}

void freeHashTable(HashTable* hashTable) {
    HashTable start = *hashTable;
    
    for (; *(*hashTable) != NULL; ++(*hashTable)) {
        free((*(*hashTable))->key);
        free(*(*hashTable));
    }

    free(start);
    *hashTable = NULL;
}

int main() {
    hashTable = initHashTable(hashMapSize);
    _print(hashTable, true);

    _set(hashTable, "key", 6);
    _set(hashTable, "yek", 10);
    _set(hashTable, "keys", 10);
    _print(hashTable, true);
    _set(hashTable, "keys11212123412123", 10);
    _print(hashTable, true);
    _set(hashTable, "keys2", 10);
    _set(hashTable, "keys3", 10);
    _set(hashTable, "keys4", 10);
    _set(hashTable, "keys5", 10);
    _set(hashTable, "keys6", 10);

    _print(hashTable, true);
    _set(hashTable, "ton", 125);
    _print(hashTable, true);

    printf("keys: %d\n", _get(hashTable, "keys"));
    printf("not: %d\n", _get(hashTable, "not"));
    printf("key: %d\n", _get(hashTable, "key"));

    _print(hashTable, true);
    _delete(hashTable, "key");
    printf("key: %d\n", _get(hashTable, "key"));
    _print(hashTable, false);
    
    printf("%p\n", hashTable);
    freeHashTable(&hashTable);
    printf("%p\n", hashTable);
    return 1;
}
