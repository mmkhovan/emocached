#ifndef SHARED_HASHTABLE_H
#define SHARED_HASHTABLE_H

int init_shared_hashtable();
void kv_set(const char* key, const char* value);
char* kv_get(const char* key);
void kv_delete(const char* key);

#endif
