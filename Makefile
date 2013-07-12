CC = gcc
main:main.c
	$(CC) main.c -o main -L/usr/lib/x86_64-linux-gnu -lcurl -Wl,-Bsymbolic-functions -Wl,-z,relro
