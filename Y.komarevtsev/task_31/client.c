#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SOCKET_PATH "./socket31"
#define BUFFER_SIZE 100

int main() {
    char buf[BUFFER_SIZE];
    int fd, rc;

    // Создаем сокет
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // Подключение к серверу
    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("Connection error");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server. Enter text to send ('Ctrl+D' to exit):\n");

    // Чтение данных из stdin и отправка на сервер
    while ((rc = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
        if (write(fd, buf, rc) != rc) {
            perror("Write error");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }

    if (rc < 0) {
        perror("Read error");
    }

    // Закрытие сокета
    close(fd);
    printf("Client disconnected.\n");
    return 0;
}

