#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork err\n");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        execlp("./affiliated_process", "affiliated_process", "test.txt", (char *)NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Message BEFORE the child process is completed\n");

        wait(NULL);

        printf("Message AFTER the child process is completed\n");
    }

    return 0;
}
