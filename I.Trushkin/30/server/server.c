#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>
#define MAX_LENGTH_TEXT 40

const char *red = "\033[31m";
const char *reset = "\033[0m";
const char *green = "\033[32m";
const char *purple = "\033[35m";


int main() {
    int sock = socket(AF_UNIX,SOCK_STREAM,0);

    if (sock == -1) {
        printf("%sError: failed to create socket %s\n", red, reset);
        exit(1);
    }

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "socket",sizeof(addr.sun_path)-1);

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr))==-1) {
        printf("%sError: failed to set socket address %s\n", red, reset);
        exit(1);
    }
    
    if (listen(sock,1) ==-1){
        printf("%sError: failed to listen socket %s\n", red, reset);
        exit(1);
    }
    
    printf("%sThe server ready to listen%s\n", green, reset);
    
    
    struct sockaddr_un client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    //int newSock = accept(sock, (struct sockaddr *)&client_addr, &client_addr_len);
    
    int newSock = accept(sock, NULL, NULL);
    if (newSock == -1){
        printf("%sError: failed to accept %s\n", red, reset);
    }
    
    char text[MAX_LENGTH_TEXT];
    
    while (1) {
        ssize_t bytesRead = read(newSock, text, MAX_LENGTH_TEXT);
        if (bytesRead <= 0) break; 
        text[bytesRead] = '0'; 
        for (int i = 0; i < bytesRead; i++) {
                text[i] = toupper(text[i]); 
        }
        printf("%s%s%s\n", purple, text, reset); 
    } 
    unlink("socket");
    close(sock);
}
