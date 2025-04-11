#ifndef PROTO_H
#define PROTO_H
#include <stdint.h>
#define MAGIC_ID 0xDEAD

enum Opcode {
    OP_SET = 0x01,
    OP_GET = 0x02,
    OP_DEL = 0x03
};

#pragma pack(push, 1)
typedef struct {
    uint16_t magic;
    uint8_t opcode;
    uint16_t key_len;
    uint16_t val_len;
    // [key][value][crc16]
} packet_header_t;
#pragma pack(pop)

#endif