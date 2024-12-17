#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SERVER "./qwerty123456"

void print_upper(const char* buf, int length) {
    for (int i = 0; i < length; ++i) {
        putchar(toupper(buf[i]));
    }
}

int main() {
    unlink(SERVER);

    int listener = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listener < 0) {
        perror("Socket creation error");
        exit(1);
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SERVER);

    if (bind(listener, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Binding error");
        close(listener);
        exit(2);
    }

    if (listen(listener, 7) < 0) {
        perror("Listening error");
        close(listener);
        exit(3);
    }

    printf("Server is running...\n");

    while (1) {
        int sock = accept(listener, NULL, NULL);
        if (sock < 0) {
            perror("Accept error");
            continue;
        }

        char buf[1024];
        int bytes_read;
        while ((bytes_read = recv(sock, buf, sizeof(buf), 0)) > 0) {
            print_upper(buf, bytes_read);
        }

        if (bytes_read < 0) {
            perror("Receive error");
        }

        printf("\nClient disconnected.\n");
        close(sock);
        break;  // Завершаем после отключения клиента
    }

    close(listener);
    unlink(SERVER);
    printf("Server stopped.\n");

    return 0;
}

