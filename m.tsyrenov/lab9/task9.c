#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char* argv[])
{
    if (argc==2)
    {
        pid_t pid;
        switch(pid=fork()) 
        {
            case -1:
                perror("error in fork");
                exit(1);
            case 0:
                execlp("cat", "cat", argv[1], NULL);
                exit(0);
            default:
                wait(NULL);
                printf("Child exit\n");
        }
    }
    else
    {
        perror("Using file");
    }
    exit(0);
}
