#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>
#include <signal.h>
#include <sys/time.h>
#include <poll.h>
#define MAX_LENGTH_TEXT 40

const char *red = "\033[31m";
const char *reset = "\033[0m";
const char *green = "\033[32m";
const char *purple = "\033[35m";
const char *yellow = "\033[33m";
int flagWhile = 0;

void handle_signal(int sig) {
    flagWhile = 1;
}

int main() {
    signal(SIGINT, handle_signal);

    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    int new_sock;
    struct sockaddr_un addr;
    struct sockaddr_un client_addr;
    char text[MAX_LENGTH_TEXT];

    if (sock == -1) {
        printf("%sError: failed to create socket %s\n", red, reset);
        exit(1);
    }

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "socket", sizeof(addr.sun_path) - 1);
    addr.sun_path[sizeof(addr.sun_path) - 1] = '\0'; // Ensure null-terminated

    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        printf("%sError: failed to set socket address %s\n", red, reset);
        close(sock);
        exit(1);
    }

    if (listen(sock, 3) == -1) {
        printf("%sError: failed to listen socket %s\n", red, reset);
        close(sock);
        exit(1);
    }

    struct pollfd act_set[100];
    act_set[0].fd = sock;
    act_set[0].events = POLLIN;
    act_set[0].revents = 0;
    int num_set = 1;

    printf("%sThe server ready to listen%s\n", green, reset);

    while (!flagWhile) {
        int ret = poll(act_set, num_set, 10000);
        if (ret < 0) {
            printf("%sError: poll failure %s\n", red, reset);
            exit(0);
        }
        if (ret > 0) {
            for (int i = 0; i < num_set; i++) {
                if (act_set[i].revents & POLLIN) {
                    act_set[i].revents &= ~POLLIN;

                    if (i == 0) {
                        new_sock = accept(sock, NULL, NULL);
                        if (num_set < 100) {
                            act_set[i].fd = new_sock;
                            act_set[i].events = POLLIN;
                            act_set[i].revents = 0;
                            num_set++;
                        } else {
                            printf("%sERROR: no more sockets%s\n", red, reset);
                        }
                    } else {
                        ssize_t bytesRead = read(act_set[i].fd, text, MAX_LENGTH_TEXT - 1);
                        if (bytesRead <= 0) {
                            close(i);
                        } else {
                            text[bytesRead] = '\0'; // Null-terminate the string
                            for (int j = 0; j < bytesRead; j++) {
                                text[j] = toupper(text[j]);
                            }
                            printf("%s%s%s\n", purple, text, reset);
                        }
                    }
                }
            }
        }
    }

    unlink("socket");
    close(sock);
}
