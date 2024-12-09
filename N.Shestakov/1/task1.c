#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/statfs.h>
#include <errno.h>
#include <limits.h>
// Функция для печати UID и GID
void print_user_ids() {
    uid_t real_uid = getuid();
    uid_t effective_uid = geteuid();
    gid_t real_gid = getgid();
    gid_t effective_gid = getegid();
    
    printf("Real UID: %d, Effective UID: %d\n", real_uid, effective_uid);
    printf("Real GID: %d, Effective GID: %d\n", real_gid, effective_gid);
}

// Функция для установки лидера группы процессов
void set_process_group_leader() {
    if (setpgid(0, 0) == -1) {
        perror("setpgid");
    }
}

// Функция для печати PID и других идентификаторов процессов
void print_process_ids() {
    pid_t pid = getpid();
    pid_t ppid = getppid();
    pid_t pgid = getpgid(0);
    
    printf("PID: %d, PPID: %d, PGID: %d\n", pid, ppid, pgid);
}

// Функция для получения информации о максимальном размере файла
void print_max_file_size() {
    struct statfs buf;

    if (statfs("/", &buf) != 0) {
        perror("statfs");
        return;
    }
    printf("Max file size on filesystem: %ld bytes\n", buf.f_frsize * buf.f_blocks);
}

// Функция для установки лимита на размер файла
void set_ulimit(long new_limit) {
    struct rlimit limit;
    limit.rlim_cur = new_limit;
    limit.rlim_max = new_limit;

    if (setrlimit(RLIMIT_FSIZE, &limit) == -1) {
        perror("Failed to set ulimit");
    }
}

// Функция для печати текущего лимита на размер core-файлов
void print_core_size() {
    struct rlimit limit;
    if (getrlimit(RLIMIT_CORE, &limit) == -1) {
        perror("getrlimit");
    } else {
        printf("Core file size: %lu bytes\n", limit.rlim_cur);
    }
}

// Функция для установки лимита на размер core-файлов
void set_core_size(rlim_t new_size) {
    struct rlimit limit;
    if (getrlimit(RLIMIT_CORE, &limit) == -1) {
        perror("getrlimit");
        return;
    }
    
    limit.rlim_cur = new_size;

    if (setrlimit(RLIMIT_CORE, &limit) == -1) {
        perror("setrlimit");
    }
}

int main(int argc, char *argv[]) {
    // Обработка аргументов командной строки
    if (argc < 2) {
        printf("Usage: %s [set_ulimit_new_value] [set_core_size_new_value]\n", argv[0]);
        return EXIT_FAILURE;
    }

    long new_ulimit = strtol(argv[1], NULL, 10);
    if (errno == ERANGE && (new_ulimit == LONG_MAX || new_ulimit == LONG_MIN)) {
        perror("Invalid ulimit value");
        return EXIT_FAILURE;
    }

    // Установка нового лимита на размер файла
    set_ulimit(new_ulimit);

    // Печать всех идентификаторов и лимитов
    print_user_ids();
    print_process_ids();
    print_max_file_size();
    print_core_size();

    return EXIT_SUCCESS;
}