#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define MAX_LENGTH_TEXT 40

const char *red = "033[31m";
const char *reset = "033[0m";
const char *green = "033[32m";
const char *purple = "033[35m";

int flag = 0;



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
            close(fd[1]);
            while (1) {
                ssize_t bytesRead = read(fd[0], text, MAX_LENGTH_TEXT);
                if (bytesRead <= 0 || flag) break;
                text[bytesRead] = '\0';
                for (int i = 0; i < bytesRead; i++) {
                    text[i] = toupper(text[i]);
                }
                printf("%s", text);
            }
            close(fd[0]);
            exit(0);
        default:
            close(fd[0]);
            printf("Enter text (or Ctrl+C to exit):\n");
            while (1) {
                if (fgets(tempText, sizeof(tempText), stdin) == NULL) break; // Проверка на EOF
                if (strlen(tempText) > MAX_LENGTH_TEXT - 1) {
                    printf("%sERROR: The entered text is too long! Maximum %d characters.%sn", red, MAX_LENGTH_TEXT - 1, reset);
                    continue;
                }
                write(fd[1], tempText, strlen(tempText)+1);
            }
            close(fd[1]);
            wait(NULL);
            break;
    }
    return 0;
}
