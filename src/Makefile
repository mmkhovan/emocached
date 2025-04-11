OS := $(shell uname -s)
ARCH := $(shell uname -m)
CC := gcc
EXE :=

ifeq ($(OS),Darwin)
	CC := clang
endif

ifeq ($(OS),Windows_NT)
	CC := x86_64-w64-mingw32-gcc
	EXE := .exe
endif

SRCS := main.c server.c hashtable.c crc16.c shared_hashtable.c
OBJS := $(SRCS:.c=.o)

TARGET := emocached$(EXE)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -std=c99 -c $< -o $@

clean:
	rm -f *.o $(TARGET)