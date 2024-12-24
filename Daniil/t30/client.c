#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/uds_socket"
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (connect(client_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, BUFFER_SIZE, stdin)) {
        if (write(client_fd, buffer, strlen(buffer)) == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    close(client_fd);
    return 0;
}