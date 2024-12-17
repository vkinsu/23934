all: client server

client: client.c
	gcc -o client client.c -lsocket

server: server.c
	gcc -o server server.c -std=c11 -lsocket

