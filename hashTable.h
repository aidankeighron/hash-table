#pragma once

typedef struct HashTableElement {
    int value;
    char* key;
    bool deleted;
} HashTableElement;
typedef HashTableElement** HashTable;

int hashMapSize = 4;
int capacity = 0;

HashTable initHashTable(int);
char* _print(HashTable, bool);
void _set(HashTable*, char*, int);
void grow(HashTable*);
int _get(HashTable, char*);
void _del(HashTable, char*);
void freeHashTable(HashTable*);