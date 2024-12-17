#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>

#define SOCKET_PATH "/tmp/ud_socket"
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

void to_uppercase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = toupper((unsigned char)str[i]);
    }
}

int main()
{
    int server_fd, client_fd, max_fd;
    struct sockaddr_un server_addr;
    fd_set read_fds, active_fds;

    // Создаем сокет
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Удаляем старый сокет, если он существует
    unlink(SOCKET_PATH);

    // Настраиваем адрес сокета
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Привязываем сокет
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Начинаем слушать соединения
    if (listen(server_fd, MAX_CLIENTS) == -1)
    {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is running, it's waiting for connect...\n");

    // Инициализируем множества файловых дескрипторов
    FD_ZERO(&active_fds);
    FD_SET(server_fd, &active_fds);
    max_fd = server_fd;

    while (1)
    {
        read_fds = active_fds;

        // Используем select для мультиплексирования
        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1)
        {
            perror("select");
            close(server_fd);
            unlink(SOCKET_PATH);
            exit(EXIT_FAILURE);
        }

        // Проверяем дескрипторы на готовность
        for (int i = 0; i <= max_fd; i++)
        {
            if (FD_ISSET(i, &read_fds))
            {
                if (i == server_fd)
                {
                    // Новое входящее соединение
                    if ((client_fd = accept(server_fd, NULL, NULL)) == -1)
                    {
                        perror("accept");
                        continue;
                    }
                    printf("New client connected: ID%d\n", client_fd);
                    FD_SET(client_fd, &active_fds);
                    if (client_fd > max_fd)
                    {
                        max_fd = client_fd;
                    }
                }
                else
                {
                    // Данные от клиента
                    char buffer[BUFFER_SIZE];
                    ssize_t num_bytes = read(i, buffer, sizeof(buffer) - 1);

                    if (num_bytes <= 0)
                    {
                        if (num_bytes == 0)
                        {
                            printf("Client disconnected: ID %d\n", i);
                        }
                        else
                        {
                            perror("read");
                        }
                        close(i);
                        FD_CLR(i, &active_fds);
                    }
                    else
                    {
                        buffer[num_bytes] = '\0';
                        to_uppercase(buffer);
                        printf("Client %d sent: %s\n", i, buffer);
                    }
                }
            }
        }
    }

    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}