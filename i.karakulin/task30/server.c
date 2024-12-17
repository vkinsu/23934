#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <ctype.h>

#define SOCKET_PATH "unix_socket"
#define BUFFER_SIZE 1024

void to_uppercase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = toupper((unsigned char)str[i]);
    }
}

int main()
{
    int server_fd, client_fd;
    struct sockaddr_un server_addr;

    // Создание сокета
    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Удаление старого сокета, если существует
    unlink(SOCKET_PATH);

    // Настройка адреса сокета
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Привязка сокета
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Ожидание соединений
    if (listen(server_fd, 5) == -1)
    {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on %s\n", SOCKET_PATH);

    // Принятие соединения
    if ((client_fd = accept(server_fd, NULL, NULL)) == -1)
    {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t num_bytes;

    // Чтение данных от клиента
    while ((num_bytes = read(client_fd, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[num_bytes] = '\0';
        to_uppercase(buffer);
        printf("Received and converted to uppercase: %s\n", buffer);
    }

    if (num_bytes == -1)
    {
        perror("read");
    }

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);

    printf("Server terminated.\n");
    return 0;
}
