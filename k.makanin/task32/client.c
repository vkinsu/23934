#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define SOCKET_PATH "./socket"

int main(int argc, char *argv[]) {
    int socketFd;
    if ((socketFd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Socket failed");
        exit(-1);
    }

    struct sockaddr_un clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sun_family = AF_UNIX;
    strncpy(clientAddr.sun_path, SOCKET_PATH, sizeof(clientAddr.sun_path) - 1);

    if (connect(socketFd, (struct sockaddr *) &clientAddr, sizeof(clientAddr)) == -1) {
        perror("Connect failed");
        exit(-1);
    }

    char buffer[1];
    while (1) {
        size_t byteRead = read(socketFd, buffer, 1);
        if (byteRead > 0) {
            printf("%c", buffer[0]);
            fflush(stdout); // Очистка буфера вывода для мгновенного вывода
        }
    }

    return 0;
}