#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/uds_socket"
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

int server_fd;
int client_sockets[MAX_CLIENTS] = {0};
volatile sig_atomic_t should_exit = 0;

void handle_sigio(int sig) {
    char buffer[BUFFER_SIZE];
    int bytes_read;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] > 0) {
            bytes_read = read(client_sockets[i], buffer, BUFFER_SIZE - 1);
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                for (int j = 0; buffer[j]; j++) {
                    buffer[j] = toupper(buffer[j]);
                }
                printf("Client %d: %s", client_sockets[i], buffer);
            } else if (bytes_read == 0) {
                printf("Disconnect %d client.\n", client_sockets[i]);
                close(client_sockets[i]);
                client_sockets[i] = 0;
            }
        }
    }
}

void set_socket_async(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK | O_ASYNC);
    fcntl(fd, F_SETOWN, getpid());
}

int main() {
    struct sockaddr_un addr;
    struct sigaction sa;

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

    if (listen(server_fd, MAX_CLIENTS) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    sa.sa_handler = handle_sigio;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGIO, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    set_socket_async(server_fd);

    while (!should_exit) {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd != -1) {
            printf("Connect %d client.\n", client_fd);

            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = client_fd;
                    set_socket_async(client_fd);
                    break;
                }
            }
        }
    }

    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}

