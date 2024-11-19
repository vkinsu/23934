#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(){
    pid_t pid = fork();

    switch(pid){
        case -1:
            perror("fork");
            exit(1);

        case 0:
            exit(system("cat romeo_and_juliet.txt"));

        default:
            wait();
            printf("child process completed\n");
    }
}
