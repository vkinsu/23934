#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <poll.h>

#define SOCKET_PATH "/tmp/unix_socket"
#define BUFFER_SIZE 256
#define MAX_CLIENTS 10

int main() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];
    struct pollfd fds[MAX_CLIENTS + 1]; 
    int num_clients = 0;

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    unlink(SOCKET_PATH);

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server started, wait clients...\n");

    fds[0].fd = server_fd;
    fds[0].events = POLLIN;

    while (1) {
        int poll_count = poll(fds, num_clients + 1, -1);
        if (poll_count < 0) {
            perror("poll");
            break;
        }

        if (fds[0].revents & POLLIN) {
            client_fd = accept(server_fd, NULL, NULL);
            if (client_fd >= 0) {
                printf("Client conected\n");
                fds[num_clients + 1].fd = client_fd;
                fds[num_clients + 1].events = POLLIN;
                num_clients++;
            } else {
                perror("accept");
            }
        }

        for (int i = 1; i <= num_clients; i++) {
            if (fds[i].revents & POLLIN) {
                ssize_t n= read(fds[i].fd, buffer, sizeof(buffer) - 1);
                if (n > 0) {
                    buffer[n] = '\0';
                    to_uppercase(buffer);

                   for (int i = 0; i < n; i++) {
                        buffer[i] = toupper(buffer[i]);
                    }

                    printf("Client input: %s\n", buffer);
                } else {
                    close(fds[i].fd);
                    fds[i] = fds[num_clients];
                    num_clients--;
                }
            }
        }
    }

    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}
