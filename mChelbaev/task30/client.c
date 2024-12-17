// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/unix_socket1"

int main() {
    int client_socket;
    struct sockaddr_un server_addr;
    char buffer[1024];
    // Создаем Unix domain socket
    client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Настраиваем адрес сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    // Устанавливаем соединение с сервером
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Вводим текст для отправки серверу
    printf("Введите текст для отправки серверу: ");
    char* all_message = (char*)malloc(1);
    all_message[0] = 0;
    while (1){
        char* tmp = fgets(buffer, sizeof(buffer), stdin);
        if (tmp == NULL) {
            perror("fgets");
            close(client_socket);
            exit(EXIT_FAILURE);
        }
        all_message = strcat(all_message, tmp);
        if(tmp[strlen(tmp)-1] != 10 || strlen(tmp) == 0){
		printf("%s", all_message);
            if (send(client_socket, all_message, strlen(all_message), 0) == -1) {
                perror("send");
                close(client_socket);
                exit(EXIT_FAILURE);
            }
        }
    }
    // Закрываем сокет
    close(client_socket);

    return 0;
}

