#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>

#define SOCKET_PATH "/tmp/ud_socket"
#define BUFFER_SIZE 256

int main() {
    int server_fd, client_fd;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_len;
    char buffer[BUFFER_SIZE];

    // Создание сокета
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Установка адреса сокета
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Привязка сокета к адресу
    unlink(SOCKET_PATH); 
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Начало прослушивания
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("The server is running, waiting for a connection...\n");

    client_len = sizeof(client_addr);
    // Ожидание подключения клиента
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Чтение данных от клиента
    while (1) {
        ssize_t nbytes = read(client_fd, buffer, sizeof(buffer) - 1);
        if (nbytes <= 0) break;

        buffer[nbytes] = '\0';
      
        for (int i = 0; i < nbytes; i++) {
            buffer[i] = toupper(buffer[i]);
        }

        printf("Received from the client: %s\n", buffer);
    }

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH); 
    return 0;
}