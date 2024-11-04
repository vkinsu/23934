#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(){
    const char *filename = "datafile.vi";

    pid_t pid = fork();

    if (pid < 0) {
        perror("Ошибка fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        execlp("cat", "cat", filename, (char *)NULL);

        perror("Ошибка в подмене процесса!");
        exit(EXIT_FAILURE);
    }
    else {
        printf("Родительский процесс: подпроцесс создан\n");
        wait(NULL);
        printf("\nРодительский процесс: подпроцесс завершился.\n");
    }

    return 0;
} 