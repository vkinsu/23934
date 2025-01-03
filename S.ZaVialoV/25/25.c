#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

int main()
{
    char *string = "test text 12345";
    int size = strlen(string) + 1;
    int fd[2]; // file descriptors. fd[0] - reading, fd[1] - writing
    if (pipe(fd) == -1)
    {
        perror("\nCould not create a pipe\n");
        exit(1);
    }
    pid_t pid = fork();
    // parallel
    if (pid < 0)
    {
        perror("\nCould not fork\n");
        exit(1);
    }
    else if (pid > 0)
    {
        close(fd[0]);
        if (write(fd[1], string, size) == -1)
        {
            perror("\nCould not write\n");
            exit(1);
        }
        close(fd[1]);
    }
    else
    {
        // Child process
        close(fd[1]);
        char getString[size];
        if (read(fd[0], getString, size) == -1)
        {
            perror("\nCould not read\n");
            exit(1);
        }

        for (int i = 0; i < size - 1; i++)
        {
            printf("%c", toupper(getString[i]));
        }
        printf("\n");
        close(fd[0]);
    }
    return 0;
}