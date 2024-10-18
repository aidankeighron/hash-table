#define PY_SSIZE_T_CLEAN
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <Python.h>
#include "hashTable.h"

HashTable hashTable;

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

char* _print(HashTable hashTable, bool printNull) {
    printf("{");
    for (; *hashTable != NULL; ++hashTable) {
        if ((*hashTable)->key != NULL || printNull) {
            printf("%s: %d, ", (*hashTable)->key, (*hashTable)->value);
        }
    }
    printf("}\n");
}

int hash(char* key) {
    int hash = 0;
    for (; *key; ++key) {
        hash = 181 * hash + *key;
    }

    return hash % hashMapSize;
}

void _set(HashTable* hashTable, char* key, int value) {
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
            _set(&newHashTable, (*(*hashTable))->key, (*(*hashTable))->value);
            free((*(*hashTable))->key);
            free(*(*hashTable));
        }
    }

    free(start);
    *hashTable = newHashTable;
}

int _get(HashTable hashTable, char* key) {
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

void _delete(HashTable hashTable, char* key) {
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

// int main() {
//     hashTable = initHashTable(hashMapSize);
//     _set(&hashTable, "key", 5);
//     _print(hashTable, true);

//     _set(&hashTable, "key", 6);
//     _set(&hashTable, "yek", 10);
//     _set(&hashTable, "keys", 10);
//     _set(&hashTable, "ton", 125);

//     printf("keys: %d\n", _get(hashTable, "keys"));
//     printf("not: %d\n", _get(hashTable, "not"));
//     printf("key: %d\n", _get(hashTable, "key"));

//     _print(hashTable, true);
//     _delete(hashTable, "key");
//     printf("key: %d\n", _get(hashTable, "key"));
//     _print(hashTable, false);
    
//     printf("%p\n", hashTable);
//     freeHashTable(&hashTable);
//     printf("%p\n", hashTable);
//     return 1;
// }

static PyObject *hashTableError;

static PyObject* print(PyObject *self, PyObject *args) {
    PyObject* printNull;

    if (!PyArg_ParseTuple(args, "O", &printNull))
        return NULL;

    _print(hashTable, PyObject_IsTrue(printNull));
    return PyLong_FromLong(1);
}

static PyObject* set(PyObject *self, PyObject *args) {
    char* key;
    const int* value;

    if (!PyArg_ParseTuple(args, "si", &key, &value)) {
        return NULL;
    }
    printf("%s\n", key);
    _set(&hashTable, key, 1);
    return PyLong_FromLong(1);
}

static PyObject* get(PyObject *self, PyObject *args) {
    char* key;

    if (!PyArg_ParseTuple(args, "s", &key))
        return NULL;
    
    int value = _get(hashTable, key);
    return PyLong_FromLong(value);
}

static PyObject* delete(PyObject *self, PyObject *args) {
    char* key;

    if (!PyArg_ParseTuple(args, "s", &key))
        return NULL;
    
    _delete(hashTable, key);
    return PyLong_FromLong(1);
}

static PyMethodDef hashTableMethods[] = {
    {"print", print, METH_VARARGS,
     "Execute a shell command."},
    {"set", set, METH_VARARGS,
     "Execute a shell command."},
    {"get", get, METH_VARARGS,
     "Execute a shell command."},
    {"delete", delete, METH_VARARGS,
     "Execute a shell command."},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef hashTablemodule = {
    PyModuleDef_HEAD_INIT,
    "hashTable",   /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    hashTableMethods
};

PyMODINIT_FUNC PyInit_hashTable(void) {
    hashTable = initHashTable(hashMapSize);
    return PyModule_Create(&hashTablemodule);
}

int main(int argc, char *argv[]) {
    PyStatus status;
    PyConfig config;
    PyConfig_InitPythonConfig(&config);

    /* Add a built-in module, before Py_Initialize */
    if (PyImport_AppendInittab("hashTable", PyInit_hashTable) == -1) {
        fprintf(stderr, "Error: could not extend in-built modules table\n");
        exit(1);
    }

    /* Pass argv[0] to the Python interpreter */
    status = PyConfig_SetBytesString(&config, &config.program_name, argv[0]);
    if (PyStatus_Exception(status)) {
        goto exception;
    }

    /* Initialize the Python interpreter.  Required.
       If this step fails, it will be a fatal error. */
    status = Py_InitializeFromConfig(&config);
    if (PyStatus_Exception(status)) {
        goto exception;
    }
    PyConfig_Clear(&config);

    /* Optionally import the module; alternatively,
       import can be deferred until the embedded script
       imports it. */
    PyObject *pmodule = PyImport_ImportModule("hashTable");
    if (!pmodule) {
        PyErr_Print();
        fprintf(stderr, "Error: could not import module 'hashTable'\n");
    }

    // ... use Python C API here ...

    return 0;

  exception:
     PyConfig_Clear(&config);
     Py_ExitStatusException(status);
}