#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "proto.h"

#define SHM_NAME "/emocached_kv"
#define SHM_SIZE (sizeof(HashTable))
#include "proto.h"

unsigned hash_key(const char* key) {
    unsigned hash = 5381;
    int c;
    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

static HashTable* shared_table = NULL;

int init_shared_hashtable() {
    shm_unlink(SHM_NAME); // cleanup old segment if it exists
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) return 0;
    if (ftruncate(fd, SHM_SIZE) == -1) return 0;

    void* map = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) return 0;

    shared_table = (HashTable*)map;
    return 1;
}

void kv_set(const char* key, const char* value) {
    unsigned hash = hash_key(key);
    strncpy(shared_table->entries[hash % TABLE_SIZE].key, key, MAX_KEY_LEN);
    strncpy(shared_table->entries[hash % TABLE_SIZE].value, value, MAX_VAL_LEN);
    shared_table->entries[hash % TABLE_SIZE].used = 1;
}

char* kv_get(const char* key) {
    unsigned hash = hash_key(key);
    Entry* e = &shared_table->entries[hash % TABLE_SIZE];
    if (e->used && strcmp(e->key, key) == 0) return e->value;
    return NULL;
}

void kv_delete(const char* key) {
    unsigned hash = hash_key(key);
    Entry* e = &shared_table->entries[hash % TABLE_SIZE];
    if (e->used && strcmp(e->key, key) == 0) e->used = 0;
}
