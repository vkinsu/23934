#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define MAX_LENGTH_TEXT 40

const char *red = "\033[31m";
const char *reset = "\033[0m";
const char *green = "\033[32m";
const char *purple = "\033[35m";

int main() {
    pid_t pid;
    int fd[2];
    char text[MAX_LENGTH_TEXT];
    char tempText[MAX_LENGTH_TEXT];

    if (pipe(fd) == -1) {
        printf("%sError: failed to create pipe %sn", red, reset);
        exit(1);
    }

    switch (pid = fork()) {
        case -1:
            printf("%sError: failed to create subprocess %sn", red, reset);
            exit(1);
        case 0:
            close(fd[1]); // Закрываем запись в pipe
            while (1) {
                ssize_t bytesRead = read(fd[0], text, MAX_LENGTH_TEXT);
                if (bytesRead <= 0) break; // Завершение при ошибке или EOF
                text[bytesRead] = '0'; // Завершение строки
                for (int i = 0; i < bytesRead; i++) {
                    text[i] = toupper(text[i]); // Преобразование в верхний регистр
                }
                printf("%s%s%s\n", purple, text, reset); // Вывод преобразованного текста
            }
            close(fd[0]); // Закрываем чтение из pipe
            exit(0);
        default:
            close(fd[0]); // Закрываем чтение из pipe
            printf("%sEnter text (or Ctrl+C to exit)%s:\n", red, reset);
            while (1) {
                if (fgets(tempText, sizeof(tempText), stdin) == NULL) break; // Проверка на EOF
                if (strlen(tempText) > MAX_LENGTH_TEXT - 1) {
                    printf("%sERROR: The entered text is too long! Maximum %d characters.%sn", red, MAX_LENGTH_TEXT - 1, reset);
                    continue;
                }
                // Удаляем символ новой строки, если он присутствует
                size_t len = strlen(tempText);
                if (len > 0 && tempText[len - 1] == '\n') {
                    tempText[len - 1] = '\0'; // Удаляем символ новой строки
                }
                write(fd[1], tempText, strlen(tempText) + 1); // Передаем строку с нулевым терминатором
            }
            close(fd[1]); // Закрываем запись в pipe
            wait(NULL); // Ожидание завершения дочернего процесса
            break;
    }
    return 0;
}
