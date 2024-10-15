#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Global
int hashMapSize = 10;

typedef struct HashTableElement {
    int value;
    char* key;
    bool deleted;
} HashTableElement;

HashTableElement** initHashTable(int size) {
    if (size <= 0) {
        printf("Invalid Size");
        return NULL;
    }
    HashTableElement** hashTable = (HashTableElement**)calloc(size+1, sizeof(HashTableElement*));
    for (int i = 0; i < size; ++i) {
        HashTableElement* element = (HashTableElement*)calloc(1, sizeof(HashTableElement));
        // element->key = (char*)calloc(100, sizeof(char));
        element->key = NULL;
        element->value = 0;
        element->deleted = false;
        hashTable[i] = element;
    }

    hashTable[size] = NULL;

    return hashTable;
}

void printHashTable(HashTableElement** hashTable) {
    printf("{");
    for (; *hashTable != NULL; ++hashTable) {
        if ((*hashTable)->key != NULL) {
            printf("%s: %d, ", (*hashTable)->key, (*hashTable)->value);
        }
    }
    printf("%p", (*hashTable));
    printf("}\n");
}

int hash(char* key) {
    int hash = 0;

    for (; *key; ++key) {
        hash = 181 * hash + *key;
    }

    return hash % hashMapSize;
}

void insert(HashTableElement** hashTable, char* key, int value) {
    int index = hash(key);

    while (hashTable[index]->key != NULL && *hashTable[index]->key != *key) {
        index = ++index % hashMapSize;
    }

    if (hashTable[index]->key == NULL) {
        hashTable[index]->key = (char *)calloc(100, sizeof(char));
    }
    strncpy(hashTable[index]->key, key, 100);
    hashTable[index]->value = value;
}

int get(HashTableElement** hashTable, char* key) {
    int index = hash(key);

    while (hashTable[index]->key == NULL && hashTable[index]->deleted || (hashTable[index]->key != NULL && *hashTable[index]->key != *key)) {
        index = ++index % hashMapSize;
    }
    if (hashTable[index]->key == NULL) {
        printf("Not found\n");
        return -1;
    }

    return hashTable[index]->value;
}

void delete(HashTableElement** hashTable, char* key) {
    int index = hash(key);

    while (hashTable[index]->key == NULL && hashTable[index]->deleted || (hashTable[index]->key != NULL && *hashTable[index]->key != *key)) {
        index = ++index % hashMapSize;
    }
    if (hashTable[index]->key == NULL) {
        printf("Not found\n");
        return;
    }

    free(hashTable[index]->key);
    hashTable[index]->key = NULL;
    hashTable[index]->value = 0;
    hashTable[index]->deleted = true;
}

void freeHashTable(HashTableElement** hashTable) {
    HashTableElement** start = hashTable;
    
    for (; *hashTable != NULL; ++hashTable) {
        free((*hashTable)->key);
        free(*hashTable);
    }

    free(start);
}

int main() {
    HashTableElement** hashTable = initHashTable(hashMapSize);
    printHashTable(hashTable);

    insert(hashTable, "key", 5);
    insert(hashTable, "key", 6);
    insert(hashTable, "yek", 10);
    insert(hashTable, "keys", 10);

    printf("keys: %d\n", get(hashTable, "keys"));
    printf("not: %d\n", get(hashTable, "not"));
    printf("key: %d\n", get(hashTable, "key"));

    printHashTable(hashTable);
    delete(hashTable, "key");
    printf("key: %d\n", get(hashTable, "key"));
    printHashTable(hashTable);
    
    freeHashTable(hashTable);
    return 1;
}