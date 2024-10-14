#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>

// Функция для печати ошибок
void print_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int opt;
    char *endptr;
    struct rlimit rl;

    // Парсингrl
    while ((opt = getopt(argc, argv, "ispuU:C:dcvV:")) != EOF) {
        switch (opt) {
            case 'i': {
                // Печатает реальные и эффективные идентификаторы пользователя и группы
                printf("Реальный UID: %d, Эффективный UID: %d\n", getuid(), geteuid());
                printf("Реальный GID: %d, Эффективный GID: %d\n", getgid(), getegid());
                break;
            }
            case 's': {
                // Процесс становится лидером группы
                if (setpgid(0, 0) == -1) {
                    print_error("Ошибка в setpgid");
                }
                printf("Процесс теперь лидер группы.\n");
                break;
            }
            case 'p': {
                // Печатает идентификаторы процесса, процесса-родителя и группы процессов
                printf("PID: %d, PPID: %d, PGID: %d\n", getpid(), getppid(), getpgid(0));
                break;
            }
            case 'u': {
                // Печатает значение ulimit
                if (getrlimit(RLIMIT_FSIZE, &rl) == -1) {
                    print_error("Ошибка в getrlimit");
                }
                printf("Текущий ulimit (размер файла): %lu\n", rl.rlim_cur);
                break;
            }
            case 'U': {
                // Изменяет значение ulimit
                long new_ulimit = strtol(optarg, &endptr, 10);
                if (*endptr != '\0' || errno == ERANGE) {
                    fprintf(stderr, "Недопустимое значение ulimit: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                rl.rlim_cur = new_ulimit;
                rl.rlim_max = new_ulimit;
                if (setrlimit(RLIMIT_FSIZE, &rl) == -1) {
                    print_error("Ошибка в setrlimit");
                }
                printf("Новый ulimit установлен на %lu\n", new_ulimit);
                break;
            }
            case 'c': {
                // Печатает размер core-файла
                if (getrlimit(RLIMIT_CORE, &rl) == -1) {
                    print_error("Ошибка в getrlimit");
                }
                printf("Ограничение размера core-файла: %lu байт\n", rl.rlim_cur);
                break;
            }
            case 'C': {
                // Изменяет размер core-файла
                long new_core_size = strtol(optarg, &endptr, 10);
                if (*endptr != '\0' || errno == ERANGE) {
                    fprintf(stderr, "Недопустимый размер core-файла: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }
                rl.rlim_cur = new_core_size;
                rl.rlim_max = new_core_size;
                if (setrlimit(RLIMIT_CORE, &rl) == -1) {
                    print_error("Ошибка в setrlimit");
                }
                printf("Новый размер core-файла установлен на %lu байт\n", new_core_size);
                break;
            }
            case 'd': {
                // Печатает текущую рабочую директорию
                char cwd[PATH_MAX];
                if (getcwd(cwd, sizeof(cwd)) == NULL) {
                    print_error("Ошибка при получении текущей рабочей директории");
                }
                printf("Текущая рабочая директория: %s\n", cwd);
                break;
            }
            case 'v': {
                // Распечатывает переменные среды и их значения
                extern char **environ;
                char **env = environ;
                while (*env) {
                    printf("%s\n", *env);
                    env++;
                }
                break;
            }
            case 'V': {
                // Добавляет новую переменную среды или изменяет существующую
                if (putenv(optarg) != 0) {
                    print_error("Ошибка при установке переменной среды");
                }
                printf("Переменная окружения установлена: %s\n", optarg);
                break;
            }
            default: {
                // Вывод справки при неверных аргументах
                fprintf(stderr, "Использование: %s [-i] [-s] [-p] [-u] [-U новое_значение_ulimit] [-c] [-C размер] [-d] [-v] [-V имя=значение]\n", argv[0]);
                exit(EXIT_FAILURE);
            }
        }
    }

    return 0;
}
