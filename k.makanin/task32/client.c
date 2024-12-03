#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define SOCKET_PATH "./socket"
#define MAX_EVENTS 10

void setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main(int argc, char *argv[]) {
    int fdIn = 0;
    if (argc == 2) {
        if ((fdIn = open(argv[1], O_RDONLY)) == -1) {
            perror("Open failed");
            exit(-1);
        }
    }

    char buffer[BUFSIZ];
    int socketFd;
    if ((socketFd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Socket failed");
        exit(-1);
    }

    setNonBlocking(socketFd);

    struct sockaddr_un clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sun_family = AF_UNIX;
    strncpy(clientAddr.sun_path, SOCKET_PATH, sizeof(clientAddr.sun_path) - 1);

    if (connect(socketFd, (struct sockaddr *) &clientAddr, sizeof(clientAddr)) == -1) {
        perror("Connect failed");
        exit(-1);
    }

    int epollFd = epoll_create1(0);
    if (epollFd == -1) {
        perror("Epoll create failed");
        exit(-1);
    }

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = socketFd;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, socketFd, &event) == -1) {
        perror("Epoll ctl add socket failed");
        exit(-1);
    }

    struct epoll_event events[MAX_EVENTS];

    size_t byteRead;
    while ((byteRead = read(fdIn, buffer, BUFSIZ)) > 0) {
        write(socketFd, buffer, byteRead);
    }

    while (1) {
        int nfds = epoll_wait(epollFd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("Epoll wait failed");
            exit(-1);
        }

        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == socketFd) {
                while ((byteRead = read(socketFd, buffer, 1)) > 0) {
                    write(STDOUT_FILENO, buffer, byteRead);
                }
            }
        }
    }

    close(socketFd);
    return 0;
}