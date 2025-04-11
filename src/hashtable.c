#include "proto.h"

unsigned hash_key(const char* key) {
    unsigned hash = 5381;
    int c;
    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}