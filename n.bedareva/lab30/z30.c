#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>

#define SOCKET_PATH "/tmp/my_socket"
#define BUFFER_SIZE 256

int main() {
    int server_sock, client_sock;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len;
    char buffer[BUFFER_SIZE];

    server_sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    unlink(SOCKET_PATH);

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, 1) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Serwer started. Wait client\n");

    client_len = sizeof(client_addr);
    client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
    if (client_sock < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("Client connected\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        ssize_t n = read(client_sock, buffer, BUFFER_SIZE - 1);
        if (n <= 0) {
            break; 
        }

        for (int i = 0; i < n; i++) {
            buffer[i] = toupper(buffer[i]);
        }

        printf("%s\n", buffer);
    }

    close(client_sock);
    close(server_sock);
    unlink(SOCKET_PATH);

    return 0;
}
