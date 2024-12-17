#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SERVER "./qwerty123456"

int main() {
    int sock;
    struct sockaddr_un addr;

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SERVER);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Connection error");
        close(sock);
        exit(2);
    }

    printf("Enter text to send to the server ('~' to exit):\n");
    char buf[1024];
    while (fgets(buf, sizeof(buf), stdin)) {
        if (buf[0] == '~' && (buf[1] == '\0' || buf[1] == '\n')) {
            break;
        }
        if (send(sock, buf, strlen(buf), 0) < 0) {
            perror("Send error");
            break;
        }
    }

    close(sock);
    return 0;
}

