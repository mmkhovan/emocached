#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "hashtable.h"

#define TABLE_SIZE (1 << 20)

typedef struct {
    char* key;
    char* value;
    int occupied;
} bucket_t;

static bucket_t* kv_table;

static uint32_t hash(const char* key) {
    uint32_t h = 0x811C9DC5;
    while (*key) h = (h ^ *key++) * 16777619;
    return h;
}

void kv_init() {
    kv_table = calloc(TABLE_SIZE, sizeof(bucket_t));
}

void kv_set(const char* key, const char* value) {
    uint32_t idx = hash(key) & (TABLE_SIZE - 1);
    while (kv_table[idx].occupied) {
        if (strcmp(kv_table[idx].key, key) == 0) {
            free(kv_table[idx].value);
            kv_table[idx].value = strdup(value);
            return;
        }
        idx = (idx + 1) & (TABLE_SIZE - 1);
    }
    kv_table[idx].key = strdup(key);
    kv_table[idx].value = strdup(value);
    kv_table[idx].occupied = 1;
}

char* kv_get(const char* key) {
    uint32_t idx = hash(key) & (TABLE_SIZE - 1);
    while (kv_table[idx].occupied) {
        if (strcmp(kv_table[idx].key, key) == 0)
            return kv_table[idx].value;
        idx = (idx + 1) & (TABLE_SIZE - 1);
    }
    return NULL;
}

void kv_delete(const char* key) {
    uint32_t idx = hash(key) & (TABLE_SIZE - 1);
    while (kv_table[idx].occupied) {
        if (strcmp(kv_table[idx].key, key) == 0) {
            free(kv_table[idx].key);
            free(kv_table[idx].value);
            kv_table[idx].occupied = 0;
            return;
        }
        idx = (idx + 1) & (TABLE_SIZE - 1);
    }
}