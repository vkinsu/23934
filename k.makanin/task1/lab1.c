#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

void print_user_ids() {
    printf("Real UID: %d, Effective UID: %d\n", getuid(), geteuid());
    printf("Real GID: %d, Effective GID: %d\n", getgid(), getegid());
}

void become_group_leader() {
    if (setpgid(0, 0) == -1) {
        perror("setpgid");
    }
}

void print_process_ids() {
    printf("Process ID: %d\n", getpid());
    printf("Parent Process ID: %d\n", getppid());
    printf("Process Group ID: %d\n", getpgrp());
}

void print_ulimit() {
    struct rlimit rlim;
    if (getrlimit(RLIMIT_NOFILE, &rlim) == 0) {
        printf("ulimit: soft=%ld, hard=%ld\n", rlim.rlim_cur, rlim.rlim_max);
    } else {
        perror("getrlimit");
    }
}

void set_ulimit(long new_ulimit) {
    struct rlimit rlim;
    if (getrlimit(RLIMIT_NOFILE, &rlim) == 0) {
        rlim.rlim_cur = new_ulimit;
        if (setrlimit(RLIMIT_NOFILE, &rlim) == -1) {
            perror("setrlimit");
        }
    } else {
        perror("getrlimit");
    }
}

void print_core_size() {
    struct rlimit rlim;
    if (getrlimit(RLIMIT_CORE, &rlim) == 0) {
        printf("Core file size: soft=%ld, hard=%ld\n", rlim.rlim_cur, rlim.rlim_max);
    } else {
        perror("getrlimit");
    }
}

void set_core_size(long size) {
    struct rlimit rlim;
    if (getrlimit(RLIMIT_CORE, &rlim) == 0) {
        rlim.rlim_cur = size;
        if (setrlimit(RLIMIT_CORE, &rlim) == -1) {
            perror("setrlimit");
        }
    } else {
        perror("getrlimit");
    }
}

void print_current_dir() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd");
    }
}

void print_env() {
    extern char **environ;
    char **env = environ;
    while (*env) {
        printf("%s\n", *env);
        env++;
    }
}

void set_env_var(char *name, char *value) {
    if (setenv(name, value, 1) == -1) {
        perror("setenv");
    }
}

int main(int argc, char *argv[]) {
    int opt;
    long new_ulimit = 0;
    long core_size = 0;
    char *env_name = NULL;
    char *env_value = NULL;

    while ((opt = getopt(argc, argv, "ispuU:cC:dveV:")) != -1) {
        switch (opt) {
            case 'i':
                print_user_ids();
                break;
            case 's':
                become_group_leader();
                break;
            case 'p':
                print_process_ids();
                break;
            case 'u':
                print_ulimit();
                break;
            case 'U':
                new_ulimit = atol(optarg);
                set_ulimit(new_ulimit);
                break;
            case 'c':
                print_core_size();
                break;
            case 'C':
                core_size = atol(optarg);
                set_core_size(core_size);
                break;
            case 'd':
                print_current_dir();
                break;
            case 'v':
                print_env();
                break;
            case 'V':
                env_name = strtok(optarg, "=");
                env_value = strtok(NULL, "=");
                if (env_name && env_value) {
                    set_env_var(env_name, env_value);
                } else {
                    fprintf(stderr, "Invalid environment variable format\n");
                }
                break;
            default:
                fprintf(stderr, "Usage: %s [-i] [-s] [-p] [-u] [-U new_ulimit] [-c] [-C size] [-d] [-v] [-V name=value]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    return 0;
}