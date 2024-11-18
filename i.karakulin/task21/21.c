#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int sigint_count = 0; // Переменная для хранения числа сигналов SIGINT

void handle_sigint(int sig)
{
    sigint_count++;
    printf("\a");
    fflush(stdout);
}

void handle_sigquit(int sig)
{
    printf("%d\n", sigint_count);
    exit(0);
}

int main()
{
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, handle_sigquit);

    while (1)
    {
        pause();
    }

    return 0;
}
