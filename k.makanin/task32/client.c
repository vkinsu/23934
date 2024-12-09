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

    char buffer[BUFSIZ];
    fd_set readFds;
    int maxFd = socketFd > STDIN_FILENO ? socketFd : STDIN_FILENO;

    while (1) {
        FD_ZERO(&readFds);
        FD_SET(socketFd, &readFds);
        FD_SET(STDIN_FILENO, &readFds);

        int activity = select(maxFd + 1, &readFds, NULL, NULL, NULL);
        if (activity == -1) {
            perror("Select failed");
            exit(-1);
        }

        if (FD_ISSET(STDIN_FILENO, &readFds)) {
            size_t bytesRead = read(STDIN_FILENO, buffer, BUFSIZ);
            if (bytesRead > 0) {
                write(socketFd, buffer, bytesRead);
            }
        }

        if (FD_ISSET(socketFd, &readFds)) {
            size_t bytesRead = read(socketFd, buffer, BUFSIZ);
            if (bytesRead > 0) {
                write(STDOUT_FILENO, buffer, bytesRead);
            }
        }
    }

    return 0;
}