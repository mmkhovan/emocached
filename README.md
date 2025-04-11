# Emocached 🖤💗 (C, CRC16, Binary Protocol)

## 🔧 Project Structure
```
src/
├── main.c           // Entry point
├── server.c         // Server and client handling
├── proto.h          // Packet structure and opcodes
├── hashtable.c      // O(1) access hash table
├── hashtable.h      // Hashtable header
├── crc16.asm        // CRC16 in x86_64 assembly
├── crc16.h          // CRC16 header
└── Makefile         // Build script
```

## 🧪 Build
Make sure you have `gcc` (or `clang` on macOS) and `nasm` installed.

```bash
cd src
make
```

This will compile the `emocached` binary for your current platform.

## 🧠 Platform Support
The `Makefile` detects your OS and architecture and applies proper flags:

- Architectures: `x86_64`, `amd64`, `arm64` (Apple Silicon)
- OS: Linux (Ubuntu, CentOS, RedHat), macOS, Windows (via MinGW)

**Examples:**
- macOS ARM64: uses `clang` + native arch
- Ubuntu x86_64: uses `gcc` + `nasm`
- Windows x86_64: uses `x86_64-w64-mingw32-gcc` + `nasm`

## 🚀 Run
```bash
./emocached
```
The server listens on TCP port `12345`.

## 📦 Protocol
Each client message is a binary packet:

```
+----------+--------+------------+---------------+------------------+
| Magic ID | OpCode | Key Length | Value Length  | Payload          |
| 2 bytes  | 1 byte | 2 bytes    | 2 bytes       | key+val          |
+----------+--------+------------+---------------+------------------+
| CRC16 (2 bytes)                                            |
+------------------------------------------------------------+
```

- **Magic ID**: `0xDEAD`
- **OpCodes**:
    - `0x01` — SET
    - `0x02` — GET
    - `0x03` — DEL
- **CRC16**: Validated over the packet body (excluding CRC field) using x86_64 assembly implementation.

## 💡 Client Example
A `client_send.py` script for crafting and sending binary packets will be added soon.

## 🧹 Clean
```bash
make clean
```
Removes object files and the binary.