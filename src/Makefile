all:
	gcc -std=c99 -c hashtable.c -o hashtable.o
	gcc -std=c99 -c server.c -o server.o
	gcc -std=c99 -c main.c -o main.o
	nasm -f elf64 crc16.asm -o crc16.o
	gcc -o memcached main.o server.o hashtable.o crc16.o

clean:
	rm -f *.o memcached