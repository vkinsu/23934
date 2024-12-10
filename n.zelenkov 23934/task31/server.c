#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>

#define SOCKET_PATH "/tmp/uds_socket"
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 5

int main() {
    int server_fd, client_fd, max_fd;
    struct sockaddr_un addr;
    fd_set read_fds, active_fds;
    char buffer[BUFFER_SIZE];
    int client_sockets[MAX_CLIENTS] = {0};

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    unlink(SOCKET_PATH);
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    FD_ZERO(&active_fds);
    FD_SET(server_fd, &active_fds);
    max_fd = server_fd;

    while (1) {
        read_fds = active_fds;

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("select");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i <= max_fd; i++) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == server_fd) {
                    client_fd = accept(server_fd, NULL, NULL);
                    if (client_fd == -1) {
                        perror("accept");
                        continue;
                    }

                    FD_SET(client_fd, &active_fds);
                    if (client_fd > max_fd) max_fd = client_fd;
                    printf("Connect %d client.\n", client_fd-3);
                } else {
                    int bytes_read = read(i, buffer, BUFFER_SIZE - 1);
                    if (bytes_read > 0) {
                        buffer[bytes_read] = '\0';
                        for (int j = 0; buffer[j]; j++) {
                            buffer[j] = toupper(buffer[j]);
                        }
                        printf("Client %d: %s", i-3, buffer);
                    } else {
                        printf("Disconnect %d client.\n", i-3);
                        close(i);
                        FD_CLR(i, &active_fds);
                    }
                }
            }
        }
    }

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);

    return 0;
}

