#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/un.h>
#include <ctype.h>
#include <fcntl.h>

#define SOCKET_PATH "/tmp/mysocket"
#define BUFFER_SIZE 256

void uppercase_and_print(char *buffer) {
    for (int i = 0; buffer[i]; i++) {
        buffer[i] = toupper(buffer[i]);
    }
    printf("Received: %s\n", buffer);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_un addr;
    char buffer[BUFFER_SIZE];
    fd_set read_fds;
    int max_fd;

    // Удаляем старый сокет, если он существует
    unlink(SOCKET_PATH);

    // Создаем сокет
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Настраиваем адрес сокета
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // Привязываем сокет к адресу
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Начинаем слушать
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("The server is running. Waiting for connections...\n");

    // Основной цикл обработки клиентов
    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);
        max_fd = server_fd;

        // Ожидаем события на сокете
        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            perror("select");
            exit(EXIT_FAILURE);
        }

        // Если событие на серверном сокете, принимаем нового клиента
        if (FD_ISSET(server_fd, &read_fds)) {
            client_fd = accept(server_fd, NULL, NULL);
            if (client_fd == -1) {
                perror("accept");
                continue;
            }

            // Читаем данные от клиента
            ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0'; // Завершаем строку
                uppercase_and_print(buffer);
            }

            close(client_fd); // Закрываем соединение с клиентом
        }
    }

    close(server_fd); // Закрываем серверный сокет (в этом коде недостижимо)
    unlink(SOCKET_PATH); // Удаляем сокет
    return 0;
}