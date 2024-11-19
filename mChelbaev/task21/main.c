#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t sigint_count = 0;

// Обработчик сигнала SIGINT
void handle_sigint(int signum) {
    sigint_count++;
    printf("\a"); 
    fflush(stdout);
}

// Обработчик сигнала SIGQUIT
void handle_sigquit(int signum) {
    printf("SIGINT received %d times.\n", sigint_count);
    exit(0);
}

int main() {
    printf("Press CTRL-C to send SIGINT and CTRL-\\ to send SIGQUIT.\n");

    while (1) {
	signal(SIGINT, handle_sigint);
    	signal(SIGQUIT, handle_sigquit);
        pause();
    }
    return 0;
}

