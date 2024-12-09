#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define BUFFER_SIZE 256

int main() {
    int pipe_fd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];

    // Создание канала
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Родительский процесс
        close(pipe_fd[0]);
        for (int i = 0; i < BUFFER_SIZE; i++){buffer[i] = '\0';}
        
        printf("Введите текст (для завершения введите 'exit'):\n");
        
        while (1) {
            fgets(buffer, BUFFER_SIZE, stdin);
            
            if (strncmp(buffer, "exit", 4) == 0) {
                break;
            }
            write(pipe_fd[1], buffer, strlen(buffer));
        }
        close(pipe_fd[1]); 
        wait(NULL); 
    } else {
        // Дочерний процесс
        close(pipe_fd[1]);

        
        while (read(pipe_fd[0], buffer, BUFFER_SIZE) > 0) {
            for (int i = 0; buffer[i]; i++) {
                buffer[i] = toupper(buffer[i]);
            }
            printf("Вверхний регистр: %s", buffer);
            for (int i = 0; i < BUFFER_SIZE; i++){buffer[i] = '\0';}
        }
        close(pipe_fd[0]);
        exit(EXIT_SUCCESS);
    }

    return 0;
}