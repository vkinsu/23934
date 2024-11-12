#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

int count;
const char *reset = "\x1b[0m";
const char *green = "\x1b[32m";
const char *yellow = "\033[33m";

void handler_SIGINT_squeak (int signum) {
    count++;
    write(STDOUT_FILENO, "\a", 1); // Издает звуковой сигнал
}

void handler_SIGQUIT_exit (int signum) {
    printf("\n%s The signal sounded: %d once \n%s",green, count, reset);
    exit(0);
}

int main() {
    printf("%sThis program beeps in the terminal every time,\n", yellow);
    printf("when the SIGINT signal (Ctr-C) is sent.\n");
    printf("The program will exit when it receives SIGQUIT (Ctr-\\).\n%s",reset);

    count = 0;
    signal(SIGINT, handler_SIGINT_squeak);
    signal(SIGQUIT, handler_SIGQUIT_exit);

    while(1) {
        pause();
    }
    return 0;
}
