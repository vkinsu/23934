#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>


int main(int argv, char* argc[]) {
    if (argv == 1) {
        printf("ERROR: Enter file, which should be cat\n");
        return 1;
    }
    printf("pid of parent process is: %i\n", getpid());
    char* args[] = { "cat", argc[1], NULL };
    pid_t child_proc, child_dummy;
    int stat;

    child_proc = fork();
    if (child_proc == (pid_t)-1) {
        printf("error with process creating: %s\n", strerror(stat));
        return 1;
    }
    if (!child_proc) {
        printf("process has been created with pid: %i\n", getpid());
        execvp("cat", args);
        exit(0);
    }
    printf("text in the main process before waiting of child\n");
    do {
        child_dummy = waitpid(child_proc, &stat, 0);
        if (child_dummy == (pid_t)-1 && errno != EINTR)
            break;
    } while (child_dummy != child_proc);
    printf("\ntext in the main process after waiting of child %i\n", getpid());
    return 0;
}
