OS := $(shell uname -s)
ARCH := $(shell uname -m)
CC := gcc
NASM := nasm
ASM_OBJ := crc16.o
EXE :=

ifeq ($(OS),Darwin)
	CC := clang
	ifeq ($(ARCH),arm64)
		CFLAGS += -target arm64-apple-macos11
	endif
endif

ifeq ($(OS),Windows_NT)
	CC := x86_64-w64-mingw32-gcc
	NASM := nasm
	EXE := .exe
endif

OBJS = main.o server.o hashtable.o $(ASM_OBJ)

all: emocached$(EXE)

emocached$(EXE): $(OBJS)
	$(CC) -o $@ $^

main.o: main.c
	$(CC) -std=c99 -c $<

server.o: server.c
	$(CC) -std=c99 -c $<

hashtable.o: hashtable.c
	$(CC) -std=c99 -c $<

crc16.o: crc16.asm
	$(NASM) -f elf64 $< -o $@

clean:
	rm -f *.o emocached$(EXE)