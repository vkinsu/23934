#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h> // Для определения pid_t
#include <sys/wait.h>  // Для wait()

#define BUFFER_SIZE 1024

int main()
{
    int pipefd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];

    if (pipe(pipefd) == -1)
    {
        return 1;
    }

    pid = fork();
    if (pid == -1)
    {
        return 1;
    }

    else if (pid == 0)
    {
        close(pipefd[1]);

        ssize_t count;
        while ((count = read(pipefd[0], buffer, BUFFER_SIZE)) > 0)
        {
            for (int i = 0; i < count; i++)
            {
                buffer[i] = toupper((unsigned char)buffer[i]);
            }
            // Выводим преобразованный текст на терминал
            write(STDOUT_FILENO, buffer, count);
        }

        // Закрываем канал на чтение
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    }
    else
    {
        close(pipefd[0]);

        const char *text = "Hello, World! This is a Test String.\n";
        write(pipefd[1], text, strlen(text));

        close(pipefd[1]);

        wait(NULL);
        exit(EXIT_SUCCESS);
    }
}
