#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main() {
    int pipe_fd[2];
    pid_t pid;

    // Создаем канал
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Создаем подпроцесс
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Дочерний процесс
        close(pipe_fd[1]);  // Закрываем конец для записи

        char buffer[BUFFER_SIZE];
        ssize_t nbytes;

        // Чтение из канала
        while ((nbytes = read(pipe_fd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[nbytes] = '\0';  // Завершаем строку

            // Переводим текст в верхний регистр
            for (int i = 0; i < nbytes; i++) {
                buffer[i] = toupper((unsigned char)buffer[i]);
            }

            // Выводим результат на терминал
            printf("Дочерний процесс получил: %s\n", buffer);
        }

        close(pipe_fd[0]);  // Закрываем конец для чтения
        exit(EXIT_SUCCESS);

    } else {  // Родительский процесс
        close(pipe_fd[0]);  // Закрываем конец для чтения

        char input[BUFFER_SIZE];
        printf("Введите текст для отправки дочернему процессу (или 'exit' для выхода):\n");

        // Чтение текста с клавиатуры и отправка его в канал
        while (1) {
            if (fgets(input, sizeof(input), stdin) == NULL) {
                break;
            }

            // Удаляем символ новой строки из конца строки, если он есть
            input[strcspn(input, "\n")] = '\0';

            // Проверяем, не введено ли 'exit' для завершения
            if (strcmp(input, "exit") == 0) {
                break;
            }

            // Отправляем текст в канал
            write(pipe_fd[1], input, strlen(input));
        }

        close(pipe_fd[1]);  // Закрываем конец для записи после завершения ввода

        // Ожидаем завершения дочернего процесса
        wait(NULL);
    }

    return 0;
}

