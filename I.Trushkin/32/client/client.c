#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>
#include <time.h>

#define MAX_LENGTH_TEXT 40

const char *red = "\033[31m";
const char *reset = "\033[0m";
const char *green = "\033[32m";
const char *purple = "\033[35m";
const char *yellow = "\033[33m";

int main() {
    int client_sock = socket(AF_UNIX, SOCK_STREAM, 0);

    if (client_sock == -1) {
        printf("%sError: failed to create socket%s\n", red, reset);
        exit(1);
    }

    printf("%sThe client is ready to connect%s\n", green, yellow);

    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "../server/socket", sizeof(addr.sun_path) - 1);
    addr.sun_path[sizeof(addr.sun_path) - 1] = '\0'; // Ensure null-termination

    if (connect(client_sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        printf("%sError: failed to connect to server%s\n", red, reset);
        close(client_sock);
        exit(1);
    }

    enum numbCklient {
        first, second
    };
    enum numbCklient id_client;

    char tempText[MAX_LENGTH_TEXT];

    // Read the initial message from the server
    read(client_sock, tempText, sizeof(tempText) - 1);
    printf("%s", tempText);

    if (strcmp(tempText, "first\n") == 0) {
        id_client = first;
    } else {
        id_client = second;
    }

    enum isMessage {
        not_received, received
    };
    enum isMessage flag = not_received;

    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 500000000; // 0.5 seconds
    int Flag = 1;
    while (Flag) {
        if (id_client == first) {
            if (flag == not_received) {
                if (fgets(tempText, sizeof(tempText), stdin) == NULL) {
                    Flag = 0;
                    break;
                };
                if (strlen(tempText) > MAX_LENGTH_TEXT - 1) {
                    printf("%sERROR: The entered text is too long! Maximum %d characters.%s\n", red, MAX_LENGTH_TEXT - 1, reset);
                    continue;
                }
                size_t len = strlen(tempText);
                if (len > 0 && tempText[len - 1] == '\n') {
                    tempText[len - 1] = '\0';
                }
                flag = received;
            }
            write(client_sock, tempText, strlen(tempText) + 1);
            nanosleep(&ts, NULL);
        } else {
            read(client_sock, tempText, sizeof(tempText) - 1);
            tempText[strlen(tempText) - 1] = '!';
            tempText[strlen(tempText)] = '\0';
            printf("%s\n", tempText);
            write(client_sock, tempText, strlen(tempText) + 1);
        }
    }

    close(client_sock);
    return 0;
}
