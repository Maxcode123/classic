#include "hashtable.h"

unsigned long ht_hash(unsigned char *key, int max_size) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % max_size;
}

HashTable ht_init(int size) {
    HashTable hash_table = (HashTable) malloc(sizeof(*hash_table));
    hash_table->entries = (Entry) malloc(sizeof(struct _Entry) * size);
    hash_table->max_size = size;
    hash_table->valid_records = 0;

    for (int i = 0; i < size; i++) {
        hash_table->entries[i].key = NULL;
        hash_table->entries[i].value = NULL;
    }

    return hash_table;
}


void ht_set(HashTable table, str key, void *value) {
    unsigned long hash = ht_hash((unsigned char *) key, table->max_size);
    while (table->entries[hash].key != NULL && strcmp(table->entries[hash].key, key) != 0) {
        hash = (hash + 1) % table->max_size;
    }
    if (table->entries[hash].key == NULL) table->valid_records++;
    table->entries[hash].key = key;
    table->entries[hash].value = value;
}

void *ht_get(HashTable table, str key) {
    unsigned long index = ht_hash((unsigned char *) key, table->max_size);
    while (table->entries[index].key != NULL && strcmp(table->entries[index].key, key) != 0) {
        index = (index + 1) % table->max_size;
    }
    if (table->entries[index].key != NULL) {
        return table->entries[index].value;
    }
    return NULL;
}

int ht_size(HashTable table) {
    return table->valid_records;
}

Iterator ht_it(HashTable table) {
    Iterator hti = malloc(sizeof(*hti));
    hti->_ht = table;
    hti->_index = 0;
    return hti;
}

bool ht_next(Iterator it) {
    HashTable ht = it->_ht;
    while (it->_index < ht->max_size) {
        size_t i = it->_index;
        it->_index++;
        if (ht->entries[i].key != NULL) {
            it->key = ht->entries[i].key;
            it->value = ht->entries[i].value;
            return true;
        }
    }
    return false;
}