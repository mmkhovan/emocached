#ifndef PROTO_H
#define PROTO_H

#include <stdint.h>

#define HEART 0xDEAD
#define OP_SET 0x01
#define OP_GET 0x02
#define OP_DEL 0x03

#define MAX_KEY_LEN 256
#define MAX_VAL_LEN 768
#define TABLE_SIZE 1024

typedef struct {
    uint16_t magic;
    uint8_t opcode;
    uint16_t key_len;
    uint16_t val_len;
} __attribute__((packed)) packet_header_t;

typedef struct {
    char key[MAX_KEY_LEN];
    char value[MAX_VAL_LEN];
    int used;
} Entry;

typedef struct {
    Entry entries[TABLE_SIZE];
} HashTable;

unsigned hash_key(const char* key);

#endif
