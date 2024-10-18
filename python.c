#include "hashTable.h"
#include <Python.h>
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

int hash(char* key) {
    int hash = 0;
    for (; *key; ++key) {
        hash = 181 * hash + *key;
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

    if (capacity >= hashMapSize) {
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
    int* value;

    if (!PyArg_ParseTuple(args, "si", &key, &value)) {
        return NULL;
    }

    _set(hashTable, key, 1);
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
    {"print", print, METH_VARARGS, "Prints hash map"},
    {"set", set, METH_VARARGS, "Adds key value pair to hash map"},
    {"get", get, METH_VARARGS, "Gets value of key from hash map"},
    {"delete", delete, METH_VARARGS, "Deletes key from hash map"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef hashTablemodule = {
    PyModuleDef_HEAD_INIT, "hashTable", NULL, -1, hashTableMethods};

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