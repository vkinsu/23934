#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int signal_count = 0;


void handle_sigint(int sig) {
    signal_count++;
}

void handle_sigquit(int sig) {
    printf("%d\n", signal_count);
    exit(0);
}

int main() {

    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, handle_sigquit);
    while (1);

    return 0;
}

