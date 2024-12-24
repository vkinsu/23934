#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s <команда> [аргументы...]\n", argv[0]);
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Ошибка fork");
        return 1;
    }
    else if (pid == 0) {
        // Дочерний процесс: выполняет команду, переданную в качестве аргументов
        execvp(argv[1], &argv[1]);
        // Если execvp вернулась, значит произошла ошибка
        perror("Ошибка execvp");
        exit(1);
    }
    else {
        // Родительский процесс: ждет завершения дочернего процесса
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("Ошибка waitpid");
            return 1;
        }

        // Проверка, завершился ли процесс нормально
        if (WIFEXITED(status)) {
            printf("Код завершения дочернего процесса: %d\n", WEXITSTATUS(status));
        } else {
            printf("Дочерний процесс завершился ненормально\n");
        }
    }

    return 0;
}