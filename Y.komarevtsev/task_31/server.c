#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <poll.h>

#define BACKLOG (5)
#define POLL_LENGTH (BACKLOG + 1)
#define BUFFER_SIZE 100
#define SOCKET_PATH "./socket31"

// Добавление нового соединения в список poll
int add_connection(struct pollfd *poll_list, int fd) {
    for (int i = 1; i < POLL_LENGTH; i++) {
        if (poll_list[i].fd < 0) {
            poll_list[i].fd = fd;
            poll_list[i].events = POLLIN | POLLPRI;
            return 0;
        }
    }
    return -1; // Список заполнен
}

int main() {
    char buf[BUFFER_SIZE];
    int fd, cl, rc;

    // Создание сокета
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    unlink(SOCKET_PATH); // Удаление старого файла сокета, если он существует

    // Привязка сокета
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("Bind error");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Прослушивание соединений
    if (listen(fd, BACKLOG) == -1) {
        perror("Listen error");
        close(fd);
        unlink(SOCKET_PATH);
        exit(EXIT_FAILURE);
    }

    // Инициализация списка poll
    struct pollfd poll_fds[POLL_LENGTH];
    for (int i = 0; i < POLL_LENGTH; i++) {
        poll_fds[i].fd = -1;
        poll_fds[i].events = POLLIN | POLLPRI;
    }

    poll_fds[0].fd = fd; // Слушающий сокет

    printf("Server is listening on %s\n", SOCKET_PATH);

    while (1) {
        // Ожидание событий на сокетах
        int poll_res = poll(poll_fds, POLL_LENGTH, -1);
        if (poll_res == -1) {
            perror("Poll error");
            break;
        }

        // Проверка событий
        for (int i = 0; i < POLL_LENGTH; i++) {
            if (poll_fds[i].fd < 0) continue; // Пропуск неактивных дескрипторов

            short revents = poll_fds[i].revents;

            // Ошибки на сокете
            if (revents & (POLLERR | POLLHUP | POLLNVAL)) {
                printf("Closing socket %d due to error\n", poll_fds[i].fd);
                close(poll_fds[i].fd);
                poll_fds[i].fd = -1;
                continue;
            }

            // Новое соединение
            if (i == 0 && (revents & POLLIN)) {
                cl = accept(fd, NULL, NULL);
                if (cl == -1) {
                    perror("Accept error");
                    continue;
                }
                printf("New client connected: %d\n", cl);

                if (add_connection(poll_fds, cl) == -1) {
                    printf("Connection limit reached, closing client: %d\n", cl);
                    close(cl);
                }
                continue;
            }

            // Данные от клиента
            if (revents & POLLIN) {
                rc = read(poll_fds[i].fd, buf, sizeof(buf));
                if (rc > 0) {
                    for (int j = 0; j < rc; j++) {
                        buf[j] = toupper(buf[j]);
                        putchar(buf[j]);
                    }
                    fflush(stdout); // Обеспечивает немедленный вывод
                } else if (rc == 0) {
                    printf("\nClient disconnected: %d\n", poll_fds[i].fd);
                    close(poll_fds[i].fd);
                    poll_fds[i].fd = -1;
                } else {
                    perror("Read error");
                    close(poll_fds[i].fd);
                    poll_fds[i].fd = -1;
                }
            }
        }
    }

    // Закрытие всех сокетов
    for (int i = 0; i < POLL_LENGTH; i++) {
        if (poll_fds[i].fd >= 0) {
            close(poll_fds[i].fd);
        }
    }

    close(fd);
    unlink(SOCKET_PATH); // Удаление файла сокета
    printf("Server stopped.\n");

    return 0;
}

