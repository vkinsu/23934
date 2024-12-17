#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int count = 0;

void handle_signal(int sig)
{
    if (sig == SIGQUIT)
    {
        char buf[50];
        int len = snprintf(buf, sizeof(buf), "\nBeep sounded: %d times\n", count);
        write(STDOUT_FILENO, buf, len);
        exit(0);
    }

    else if (sig == SIGINT)
    {
        write(STDOUT_FILENO, "\007", 1);
        count++;
        signal(SIGINT, handle_signal);
    }
}

int main()
{
    signal(SIGINT, handle_signal);
    signal(SIGQUIT, handle_signal);

    while (1)
        pause();

    return 0;
}