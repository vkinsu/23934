#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/mysocket"
#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    int client_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <text>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Создаем сокет
    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Настраиваем адрес сокета
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // Устанавливаем соединение с сервером
    if (connect(client_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // Отправляем данные на сервер
    snprintf(buffer, BUFFER_SIZE, "%s\n", argv[1]);
    write(client_fd, buffer, strlen(buffer));

    close(client_fd); // Закрываем соединение
    return 0;
}