import socket
import struct
import sys

HEART = 0xDEAD
OP_SET = 0x01
OP_GET = 0x02
OP_DEL = 0x03

def crc16(data):
    crc = 0
    for b in data:
        crc ^= b << 8
        for _ in range(8):
            if crc & 0x8000:
                crc = (crc << 1) ^ 0x1021
            else:
                crc <<= 1
        crc &= 0xFFFF
    return crc

def make_packet(opcode, key, value=b""):
    key = key.encode() if isinstance(key, str) else key
    value = value.encode() if isinstance(value, str) else value
    header = struct.pack(">H B H H", HEART, opcode, len(key), len(value))
    body = key + value
    checksum = struct.pack(">H", crc16(header + body))
    return header + body + checksum

def send_packet(pkt):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        try:
            s.connect(("127.0.0.1", 12345))
            s.sendall(pkt)
            response = s.recv(1024)
            if response:
                print("[Response]", response.decode(errors="replace"))
            else:
                print("[Warning] No response received")
        except Exception as e:
            print("[Error]", e)

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python3 client_send.py <set|get|del> <key> [value]")
        sys.exit(1)

    cmd = sys.argv[1].lower()
    key = sys.argv[2]
    value = sys.argv[3] if len(sys.argv) > 3 else ""

    if cmd == "set":
        print(f"[SET] key='{key}' value='{value}'")
        send_packet(make_packet(OP_SET, key, value))
    elif cmd == "get":
        print(f"[GET] key='{key}'")
        send_packet(make_packet(OP_GET, key))
    elif cmd == "del":
        print(f"[DEL] key='{key}'")
        send_packet(make_packet(OP_DEL, key))
    else:
        print("[Error] Unknown command. Use: set, get, del")
