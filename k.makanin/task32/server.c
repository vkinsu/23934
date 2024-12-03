#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include <sys/epoll.h>

#define SOCKET_PATH "./socket"
#define MAX_CLIENTS 10

void sigCatch(int sig) {
    unlink(SOCKET_PATH);
    _exit(1);
}

int main() {
    char buffer[BUFSIZ];
    int socketFd;
    if ((socketFd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Socket failed");
        exit(-1);
    }

    struct sockaddr_un serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sun_family = AF_UNIX;
    strncpy(serverAddr.sun_path, SOCKET_PATH, sizeof(serverAddr.sun_path) - 1);

    if (bind(socketFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
        perror("Bind failed");
        exit(-1);
    }
    signal(SIGINT, sigCatch);
    signal(SIGQUIT, sigCatch);

    if (listen(socketFd, MAX_CLIENTS) == -1) {
        unlink(SOCKET_PATH);
        perror("Listen error");
        exit(-1);
    }

    int epollFd = epoll_create1(0);
    if (epollFd == -1) {
        unlink(SOCKET_PATH);
        perror("Epoll create failed");
        exit(-1);
    }

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = socketFd;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, socketFd, &event) == -1) {
        unlink(SOCKET_PATH);
        perror("Epoll ctl failed");
        exit(-1);
    }

    struct epoll_event events[MAX_CLIENTS + 1];

    while (1) {
        int nfds = epoll_wait(epollFd, events, MAX_CLIENTS + 1, -1);
        if (nfds == -1) {
            unlink(SOCKET_PATH);
            perror("Epoll wait failed");
            exit(-1);
        }

        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == socketFd) {
                int clientFd = accept(socketFd, NULL, NULL);
                if (clientFd == -1) {
                    perror("Accept failed");
                    unlink(SOCKET_PATH);
                    exit(-1);
                }

                event.events = EPOLLIN;
                event.data.fd = clientFd;
                if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &event) == -1) {
                    perror("Epoll ctl add client failed");
                    close(clientFd);
                }
            } else {
                int clientFd = events[i].data.fd;
                size_t bytesRead = read(clientFd, buffer, BUFSIZ);
                if (bytesRead <= 0) {
                    close(clientFd);
                    epoll_ctl(epollFd, EPOLL_CTL_DEL, clientFd, NULL);
                } else {
                    for (size_t j = 0; j < bytesRead; j++) {
                        putc(toupper(buffer[j]), stdout);
                    }
                }
            }
        }
    }
}
