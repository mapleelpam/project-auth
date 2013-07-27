CC = gcc
main:main.c auth-client.c constants.h
	$(CC) -c auth-client.c -o auth-client.o
	$(CC) -c main.c -o main.o
	$(CC) auth-client.o main.o -o main -lcurl
	rm *.o
#-L/usr/lib/x86_64-linux-gnu -lcurl -Wl,-Bsymbolic-functions -Wl,-z,relro
