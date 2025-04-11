#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "proto.h"
#include "crc16.c"
#include "shared_hashtable.h"

#define PORT 12345
#define MAX_PACKET 2048

void handle_client(int sock) {
    uint8_t buffer[MAX_PACKET];
    ssize_t n = recv(sock, buffer, sizeof(buffer), 0);
    printf("[Server] Received %zd bytes\n", n);
    if (n <= sizeof(packet_header_t) + 2) {
        printf("[Server] Packet too short, skipping\n");
        return;
    }

    printf("[Server] New client connected\n");

    packet_header_t* hdr = (packet_header_t*) buffer;
    if (ntohs(hdr->magic) != HEART) {
        printf("[Server] Invalid HEART (magic) received: 0x%04X\n", ntohs(hdr->magic));
        return;
    }

    size_t payload_len = ntohs(hdr->key_len) + ntohs(hdr->val_len);
    uint16_t received_crc;
    memcpy(&received_crc, buffer + sizeof(packet_header_t) + payload_len, 2);
    received_crc = (received_crc << 8) | (received_crc >> 8); // Fix endianness
    uint16_t computed_crc = crc16(buffer, sizeof(packet_header_t) + payload_len);

    printf("[Server] Validating CRC: received 0x%04X vs expected 0x%04X\n", received_crc, computed_crc);

    if (received_crc != computed_crc) {
        printf("[Server] CRC mismatch!\n");
        send(sock, "ERR_CRC", 7, 0);
        return;
    }

    char key[1024] = {0}, val[1024] = {0};
    memcpy(key, buffer + sizeof(packet_header_t), ntohs(hdr->key_len));
    if (hdr->opcode == OP_SET)
        memcpy(val, buffer + sizeof(packet_header_t) + ntohs(hdr->key_len), ntohs(hdr->val_len));

    printf("[Server] Opcode: 0x%02X, Key: '%s'\n", hdr->opcode, key);
    if (hdr->opcode == OP_SET) printf("[Server] SET val='%s'\n", val);

    switch (hdr->opcode) {
        case OP_SET:
            kv_set(key, val);
            send(sock, "OK", 2, 0);
            break;
        case OP_GET: {
            printf("[Server] GET lookup for key='%s'\n", key);
            char* found = kv_get(key);
            if (found)
                send(sock, found, strlen(found), 0);
            else
                send(sock, "NOTFOUND", 8, 0);
            break;
        }
        case OP_DEL:
            kv_delete(key);
            send(sock, "DELETED", 7, 0);
            break;
        default:
            send(sock, "UNKNOWN", 7, 0);
    }
}

void start_server() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = INADDR_ANY
    };

    bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    listen(sockfd, 5);
    printf("[Server] Listening on port %d...\n", PORT);

    if (!init_shared_hashtable()) {
        fprintf(stderr, "[Server] Failed to init shared hashtable\n");
        exit(1);
    }

    while (1) {
        int client = accept(sockfd, NULL, NULL);
        if (fork() == 0) {
            close(sockfd);
            handle_client(client);
            close(client);
            exit(0);
        }
        close(client);
    }
}
