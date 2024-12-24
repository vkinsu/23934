#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Ошибка fork");
        exit(1);
    }
    else if (pid == 0) {
        // Дочерний процесс: выполнить команду cat
        execlp("cat", "cat", "text.txt", NULL);
        perror("Ошибка execlp");
        exit(1);
    }
    else {
        // Родительский процесс: ждет завершения дочернего
        waitpid(pid, NULL, 0);
        printf("Это сообщение от родительского процесса после завершения дочернего.\n");
    }

    return 0;
}