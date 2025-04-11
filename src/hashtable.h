#ifndef HASHTABLE_H
#define HASHTABLE_H
void kv_init();
void kv_set(const char* key, const char* value);
char* kv_get(const char* key);
void kv_delete(const char* key);
#endif