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
const char *purple = "\x1b[35m";


int main(int argc, char* argv[]) {
    if (argc == 1) {
        printf("%sError: no arguments entered %s\n", red, reset);
    }
    pid_t pid;
    int status;
    switch (pid = fork()) {
        case -1:
            printf("%sError: failed to create subprocess %s\n", red, reset);
            exit(1);
        case 0:
            printf("%sCHILD: This is a child process!%s \n", green, reset);
            execvp(argv[1], &argv[1]);
            printf("%sError: failed to replace process %s", red, reset);
            exit(1);

        default:
            printf("%sPARENT: This is a parent process!%s\n", purple, reset);
            wait(NULL);
            printf("\n%s%d%s", purple, WEXITSTATUS(status), reset);
            printf("\n%sThe parent waited for the subprocess to complete!\n%s",purple,reset);
    }
    return 0;
}
