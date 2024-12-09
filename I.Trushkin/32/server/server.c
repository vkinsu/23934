#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define MAX_LENGTH_TEXT 40

const char *red = "\033[31m";
const char *reset = "\033[0m";
const char *green = "\033[32m";
const char *purple = "\033[35m";
const char *yellow = "\033[33m";

int main() {
    unlink("socket");

    int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    int new_sock;

    struct sockaddr_un addr;
    struct sockaddr_un client_addr;

    char text[MAX_LENGTH_TEXT];

    fd_set active_set, read_set;

    int size;

    if (sock == -1) {
        printf("%sError: failed to create socket %s\n", red, reset);
        exit(1);
    }

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "socket", sizeof(addr.sun_path) - 1);

    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        printf("%sError: failed to set socket address %s\n", red, reset);
        exit(1);
    }

    if (listen(sock, 3) == -1) {
        printf("%sError: failed to listen socket %s\n", red, reset);
        exit(1);
    }

    FD_ZERO(&active_set);
    FD_SET(sock, &active_set);
    int max_fd = sock;

    printf("%sThe server ready to listen%s\n", green, reset);

    char **numb_client = (char **)malloc(2 * sizeof(char *));
    numb_client[0] = (char *)malloc(7 * sizeof(char)); // "first" + '\0'
    numb_client[1] = (char *)malloc(8 * sizeof(char)); // "second" + '\0'
    strcpy(numb_client[0], "first\n");
    strcpy(numb_client[1], "second\n");

    int *idx_client = (int *)malloc(100 * sizeof(int));
    int *fd_client = (int *)malloc(2 * sizeof(int));
    char **color_client = (char**)malloc(100 * sizeof(char*));


    while (1) {
        read_set = active_set;

        if (select(max_fd + 1, &read_set, NULL, NULL, NULL) < 0) {
            printf("%sError: failed to select failure %s\n", red, reset);
            exit(1);
        }

        for (int i = 0; i <= max_fd; i++) {
            if (FD_ISSET(i, &read_set)) {
                if (i == sock) {
                    new_sock = accept(sock, NULL, NULL);
                    if (new_sock == -1) {
                        printf("%sError: failed to accept %s\n", red, reset);
                        exit(1);
                    } else {
                        FD_SET(new_sock, &active_set);
                        if (new_sock > max_fd) {
                            max_fd = new_sock;
                        }
                    }
                    if (numb_client[0] != NULL) {
                        idx_client[new_sock] = 0;
                        color_client[new_sock] = red;
                        fd_client[0] = new_sock;
                        write(new_sock, numb_client[0], strlen(numb_client[0]) + 1);
                        numb_client[0] = NULL;
                    } else {
                        idx_client[new_sock] = 1;
                        fd_client[1] = new_sock;
                        color_client[new_sock] = yellow;
                        write(new_sock, numb_client[1], strlen(numb_client[1]) + 1);
                    }
                } else {
                    ssize_t bytesRead = read(i, text, MAX_LENGTH_TEXT - 1);
                    if (bytesRead <= 0) {
                        close(i);
                        FD_CLR(i, &active_set);
                    } else {
                        text[bytesRead] = '\0';
                        printf("%s%s%s\n", color_client[i], text, reset);
                        if (idx_client[i] == 0) {
                            write(fd_client[1], text, strlen(text) + 1);
                        }
                    }
                }
            }
        }
    }
}
