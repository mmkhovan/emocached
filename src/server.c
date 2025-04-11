#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "proto.h"
#include "hashtable.h"
#include "crc16.h"

#define PORT 12345
#define MAX_PACKET 2048

void handle_client(int sock) {
    uint8_t buffer[MAX_PACKET];
    ssize_t n = recv(sock, buffer, sizeof(buffer), 0);
    if (n <= sizeof(packet_header_t) + 2) return;

    packet_header_t* hdr = (packet_header_t*) buffer;
    if (ntohs(hdr->magic) != MAGIC_ID) return;

    size_t payload_len = ntohs(hdr->key_len) + ntohs(hdr->val_len);
    uint16_t received_crc;
    memcpy(&received_crc, buffer + sizeof(packet_header_t) + payload_len, 2);
    uint16_t computed_crc = crc16(buffer, sizeof(packet_header_t) + payload_len);

    if (received_crc != computed_crc) {
        send(sock, "ERR_CRC", 7, 0);
        return;
    }

    char key[1024] = {0}, val[1024] = {0};
    memcpy(key, buffer + sizeof(packet_header_t), ntohs(hdr->key_len));
    if (hdr->opcode == OP_SET)
        memcpy(val, buffer + sizeof(packet_header_t) + ntohs(hdr->key_len), ntohs(hdr->val_len));

    switch (hdr->opcode) {
        case OP_SET:
            kv_set(key, val);
            send(sock, "OK", 2, 0);
            break;
        case OP_GET: {
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
    printf("Listening on port %d...\n", PORT);

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