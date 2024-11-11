#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

    const char *filename = "file.txt";
    pid_t pid = fork();
    

    if (pid == 0) {
        /* child */
        execlp("cat", "cat", filename, NULL);
        
    } else {
        /* parent */
        printf("Parent process\n");


        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Child process completed \n");
        } else {
            printf("Child process did not completed\n");
        }


        printf("Parent process: This message is printed after the child process has finished\n");
    }

    return 0;
}

