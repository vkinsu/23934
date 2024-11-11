#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t sigint_count = 0;

void handle_sigint(int sig) {
    sigint_count++;
    printf("\a");
}

void handle_sigquit(int sig) {
    printf("\nSIGQUIT received. SIGINT was received %d times.\n", sigint_count);
    exit(0);
}

int main() {
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, handle_sigquit);

    printf("Program running... Press CTRL-C to sound beep, Press CTRL-\\ to quit.\n");

    while (1) {
        pause();
    }

    return 0;
}