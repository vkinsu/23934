#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>

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