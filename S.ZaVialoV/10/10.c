#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char** argv){
    char* command = (char*)malloc(0);
    
    if(argc == 1){
        perror("no function\n");
        exit(1);
    }

    for(int i=1; i<argc; i++){
        command = strcat(command, argv[i]);
        command = strcat(command, " ");
    }
    
    pid_t pid = fork();

    switch(pid){
        case -1:
            perror("fork");
            exit(1);

        case 0:
            exit(system(command));
            // exit(95);

        default:
            int p;
            wait(&p);
            printf("exit code of process with pid %d is %d\n", pid, WEXITSTATUS(p));
    }
}