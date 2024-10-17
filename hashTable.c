#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Global
int hashMapSize = 4;
int capacity = 0;

typedef struct HashTableElement {
    int value;
    char* key;
    bool deleted;
} HashTableElement;
typedef HashTableElement** HashTable;

void grow(HashTable*);
void freeHashTable(HashTable*);

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

void printHashTable(HashTable hashTable, bool printNull) {
    printf("{");
    for (; *hashTable != NULL; ++hashTable) {
        if ((*hashTable)->key != NULL || printNull) {
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

void insert(HashTable* hashTable, char* key, int value) {
    int index = hash(key);

    while ((*hashTable)[index]->key != NULL && *(*hashTable)[index]->key != *key) {
        index += 1;
        index %= hashMapSize;
    }

    if ((*hashTable)[index]->key == NULL) {
        (*hashTable)[index]->key = (char *)calloc(100, sizeof(char));
        capacity += 1;
    }
    strncpy((*hashTable)[index]->key, key, 100);
    (*hashTable)[index]->value = value;

    if (capacity >= hashMapSize) {
        grow(hashTable);
    }
}

void grow(HashTable* hashTable) {
    hashMapSize *= 2;
    capacity = 0;
    HashTable newHashTable = initHashTable(hashMapSize);

    HashTable start = *hashTable;
    for (; (*(*hashTable)) != NULL; ++(*hashTable)) {
        if ((*(*hashTable))->key != NULL) {
            insert(&newHashTable, (*(*hashTable))->key, (*(*hashTable))->value);
            free((*(*hashTable))->key);
            free(*(*hashTable));
        }
    }

    free(start);
    *hashTable = newHashTable;
}

int get(HashTable hashTable, char* key) {
    int index = hash(key);

    while ((hashTable[index]->key == NULL && hashTable[index]->deleted) || 
        (hashTable[index]->key != NULL && *hashTable[index]->key != *key)) {
        index += 1;
        index %= hashMapSize;
    }
    if (hashTable[index]->key == NULL) {
        printf("Not found\n");
        return -1;
    }

    return hashTable[index]->value;
}

void delete(HashTable hashTable, char* key) {
    int index = hash(key);
    while ((hashTable[index]->key == NULL && hashTable[index]->deleted) || 
        (hashTable[index]->key != NULL && *hashTable[index]->key != *key)) {
        index += 1;
        index %= hashMapSize;
    }
    if (hashTable[index]->key == NULL) {
        printf("Not found\n");
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

// check for memory leaks: valgrind --tool=memcheck --leak-check=full ./hashTable

int main() {
    HashTable hashTable = initHashTable(hashMapSize);
    printHashTable(hashTable, true);

    insert(&hashTable, "key", 5);
    insert(&hashTable, "key", 6);
    insert(&hashTable, "yek", 10);
    insert(&hashTable, "keys", 10);
    insert(&hashTable, "ton", 125);

    printf("keys: %d\n", get(hashTable, "keys"));
    printf("not: %d\n", get(hashTable, "not"));
    printf("key: %d\n", get(hashTable, "key"));

    printHashTable(hashTable, true);
    delete(hashTable, "key");
    printf("key: %d\n", get(hashTable, "key"));
    printHashTable(hashTable, false);
    
    printf("%p\n", hashTable);
    freeHashTable(&hashTable);
    printf("%p\n", hashTable);
    return 1;
}