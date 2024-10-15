#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// gcc hash_table.c

typedef struct HashTableElement {
    int value;
    char* key;
} HashTableElement;

HashTableElement** initHashTable(int size) {
    if (size <= 0) {
        printf("Invalid Size");
        return NULL;
    }
    HashTableElement** hashTable = (HashTableElement**)calloc(size+1, sizeof(HashTableElement*));
    for (int i = 0; i < size; ++i) {
        HashTableElement* element = (HashTableElement*)calloc(1, sizeof(HashTableElement));
        element->key = "test";
        element->value = i;
        hashTable[i] = element;
    }

    hashTable[size] = NULL;

    return hashTable;
}

void printHashTable(HashTableElement** hashTable) {
    printf("{");
    for (; *hashTable != NULL; ++hashTable) {
        printf("%s: %d, ", (*hashTable)->key, (*hashTable)->value);
    }
    printf("%p", (*hashTable));
    printf("}\n");
}

void insert(HashTableElement** hashTable, char* key, int value) {
    // strncpy(hashTable[value]->key, key, 99); 
    hashTable[value]->value = 10;
}

int main() {
    HashTableElement** hashTable = initHashTable(10);
    printHashTable(hashTable);
    insert(hashTable, "key", 5);
    printHashTable(hashTable);
    return 1;
}