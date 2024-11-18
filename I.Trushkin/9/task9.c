#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define FILE_NAME_LEN 100

const char *red = "\x1b[31m";
const char *reset = "\x1b[0m";
const char *green = "\x1b[32m";
const char *purple = "\x1b[35m";switch (pid = fork()) {
        case -1:
            printf("%sError: failed to create subprocess %s", red, reset);
            exit(1);
        case 0:
            printf("%sCHILD: This is a child process!\n", green, reset);
            printf("Enter file name, please:\n");
            if (fgets(filename, FILE_NAME_LEN, stdin) == NULL) {
                printf("%sError: failed to read file name%s", red, reset);
                exit(1);
            } else {
                size_t len = strlen(filename);
                if (len > 0 && filename[len - 1] == '\n') {
                    filename[len - 1] = '\0';
                }
            }
            execlp("cat", "cat", filename, (char *) 0);
            printf("%sError: failed to replace process %s", red, reset);
            exit(1);

        default:
            printf("%sPARENT: This is a parent process!%s\n", purple, reset);
            wait(NULL);
            printf("\n%sThe parent waited for the subprocess to complete!\n%s",purple,reset);
    }
    return 0;
}



int main() {
    pid_t pid;
    char filename[FILE_NAME_LEN];

