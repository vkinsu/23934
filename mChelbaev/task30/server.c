// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>
#include <sys/types.h>

#define SOCKET_PATH "/tmp/unix_socket1"

int main() {
    int server_socket, client_socket;
    struct sockaddr_un server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[1024];
    int bytes_received;

    // Создаем Unix domain socket
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Удаляем сокет, если он уже существует
    unlink(SOCKET_PATH);

    // Настраиваем адрес сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Привязываем сокет к пути
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Начинаем слушать на сокете
    if (listen(server_socket, 1) == -1) {
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Принимаем соединение от клиента
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket == -1) {
        perror("accept");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Получаем данные от клиента
    bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received == -1) {
        perror("recv");
        close(client_socket);
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    buffer[bytes_received] = '\0';

    // Переводим текст в верхний регистр
    for (int i = 0; i < bytes_received; i++) {
        buffer[i] = toupper(buffer[i]);
    }

    // Выводим текст в стандартный поток вывода
    printf("Полученный текст в верхнем регистре: %s\n", buffer);

    // Закрываем сокеты
    close(client_socket);
    close(server_socket);

    // Удаляем сокет
    unlink(SOCKET_PATH);

    return 0;
}

