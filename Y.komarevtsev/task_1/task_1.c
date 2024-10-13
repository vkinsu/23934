#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <sys/utsname.h>

extern char **environ;

void print_ids() {
    printf("Real UID: %d, Effective UID: %d\n", getuid(), geteuid());
    printf("Real GID: %d, Effective GID: %d\n", getgid(), getegid());
}

void set_leader() {
    if (setpgid(0, 0) == 0) {
        printf("Process became a group leader.\n");
    } else {
        perror("Failed to become group leader");
    }
}

void print_process_ids() {
    printf("Process ID: %d, Parent Process ID: %d, Group Process ID: %d\n", getpid(), getppid(), getpgrp());
}

void print_ulimit() {
    struct rlimit rl;
    if (getrlimit(RLIMIT_FSIZE, &rl) == 0) {
        printf("Current file size limit: %lu bytes\n", rl.rlim_cur);
    } else {
        perror("Failed to get file size limit");
    }
}

void set_ulimit(char *optarg) {
    struct rlimit rl;
    rl.rlim_cur = atol(optarg); // Преобразование аргумента в число
    rl.rlim_max = RLIM_INFINITY; // Устанавливаем максимальный лимит

    if (setrlimit(RLIMIT_FSIZE, &rl) == 0) {
        printf("File size limit changed to: %ld bytes\n", rl.rlim_cur);
    } else {
        perror("Failed to change file size limit");
    }
}

void print_core_limit() {
    struct rlimit rl;
    if (getrlimit(RLIMIT_CORE, &rl) == 0) {
        printf("Core file size limit: %ld bytes\n", rl.rlim_cur);
    } else {
        perror("Failed to get core file size limit");
    }
}

void set_core_limit(char *optarg) {
    struct rlimit rl;
    if (getrlimit(RLIMIT_CORE, &rl) == 0) {
        rl.rlim_cur = atol(optarg);
        if (setrlimit(RLIMIT_CORE, &rl) == 0) {
            printf("Core file size limit changed to: %ld bytes\n", rl.rlim_cur);
        } else {
            perror("Failed to set core file size limit");
        }
    } else {
        perror("Failed to get core file size limit");
    }
}

void print_working_directory() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("Failed to get current working directory");
    }
}

void print_environment() {
    char **env;
    for (env = environ; *env != 0; env++) {
        printf("%s\n", *env);
    }
}

void set_environment_variable(char *optarg) {
    if (putenv(optarg) != 0) {
        perror("Failed to set environment variable");
    } else {
        printf("Environment variable set: %s\n", optarg);
    }
}

int main(int argc, char *argv[]) {
    int opt;
    char *options = "ispucC:U:dvV:";
    
    // Обрабатываем опции справа налево
    while ((opt = getopt(argc, argv, options)) != -1) {
        switch (opt) {
            case 'i':
                print_ids();
                break;
            case 's':
                set_leader();
                break;
            case 'p':
                print_process_ids();
                break;
            case 'u':
                print_ulimit();
                break;
            case 'U':
                set_ulimit(optarg);
                break;
            case 'c':
                print_core_limit();
                break;
            case 'C':
                set_core_limit(optarg);
                break;
            case 'd':
                print_working_directory();
                break;
            case 'v':
                print_environment();
                break;
            case 'V':
                set_environment_variable(optarg);
                break;
            case '?':
                fprintf(stderr, "Unknown option: -%c\n", optopt);
                break;
        }
    }
    return 0;
}

