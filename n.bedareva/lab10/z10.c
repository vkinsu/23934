#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    pid_t pid = fork();


    if (pid == 0) {
         /* child */
        execvp(argv[1], &argv[1]);

    } else {
        /* parent */
        
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Child process completed");
        } else {
            printf("Child process did not completed");
        }
    }

    return 0;
}

