#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct _Entry {
    void *key;
    void *value;
} *Entry;

typedef struct _HashTable {
    struct _Entry *entries;
    int max_size;
    int valid_records; //todo: is it necessary?
} *HashTable;

typedef struct _Iterator {
    char *key;
    void *value;
    HashTable _ht;
    size_t _index;
} *Iterator;

// djb2 hash algorithm
unsigned long ht_hash(unsigned char *key, int max_size);

//Initialize the ht by suze
HashTable ht_init(int size);

//Insert new record to ht
void ht_insert(HashTable table, char *key, void *value);

//Gets the record by "key"
void *ht_search(HashTable table, char *key);

//Gets the size of the valid records
int ht_size(HashTable table);

// Creates iterator for given table.
Iterator ht_it(HashTable table);

// Returns true if there is a next value. Key and value are stored in iterator.
bool ht_next(Iterator it);