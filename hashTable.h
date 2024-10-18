#pragma once
#include <stdbool.h>

typedef struct HashTableElement {
    int value;
    char* key;
    bool deleted;
} HashTableElement;
typedef HashTableElement** HashTable;

HashTable hashTable;
int hashMapSize = 4;
int capacity = 0;

HashTable initHashTable(int);
void _print(HashTable, bool);
void _set(HashTable, char*, int);
int _get(HashTable, char*);
void _delete(HashTable, char*);
