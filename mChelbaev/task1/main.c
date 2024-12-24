#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <errno.h>

void print_ids() {
    printf("Real UID: %d, Effective UID: %d\n", getuid(), geteuid());
    printf("Real GID: %d, Effective GID: %d\n", getgid(), getegid());
}

void print_pids() {
    printf("Process ID: %d, Parent Process ID: %d, Group ID: %d\n", getpid(), getppid(), getpgrp());
}


void become_group_leader() {
    if (setpgid(0, 0) == -1) {
        perror("setpgid");
    } else {
        printf("Process became group leader\n");
    }
}

void print_ulimit() 
{
    struct rlimit rl;
    if (getrlimit(RLIMIT_FSIZE, &rl) == 0) {
        printf("Current ulimit: %ld\n", rl.rlim_cur);
    } else {
        perror("getrlimit");
    }

}


void set_ulimit(const char *new_ulimit) {
    struct rlimit rl;
    char *endptr;
    long val = strtol(new_ulimit, &endptr, 10);
    if (*endptr != '\0' || val < 0) {
        fprintf(stderr, "Invalid ulimit value: %s\n", new_ulimit);
        return;
    }
    rl.rlim_cur = val;
    rl.rlim_max = val;
    if (setrlimit(RLIMIT_FSIZE, &rl) == -1) {
        perror("setrlimit");
    } else {
        printf("New ulimit set to: %ld\n", val);
    }
}

void print_core_size() {
    struct rlimit rl;
    if (getrlimit(RLIMIT_CORE, &rl) == 0) {
        printf("Core file size: %ld\n", rl.rlim_cur);
    } else {
        perror("getrlimit");
    }
}

void set_core_size(const char *size) {
    struct rlimit rl;
    char *endptr;
    long val = strtol(size, &endptr, 10);
    if (*endptr != '\0' || val < 0) {
        fprintf(stderr, "Invalid core size value: %s\n", size);
        return;
    }
    rl.rlim_cur = val;
    rl.rlim_max = val;
    if (setrlimit(RLIMIT_CORE, &rl) == -1) {
        perror("setrlimit");
    } else {
        printf("Core file size set to: %ld\n", val);
    }
}

void print_cwd() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working directory: %s\n", cwd);
    } else {
        perror("getcwd");
    }
}

void print_env() {
    extern char **environ;
    for (char **env = environ; *env != 0; env++) {
        char *thisEnv = *env;
        printf("%s\n", thisEnv);
    }
}

void set_env(const char *name_value) {
    char *name = strdup(name_value);
    char *value = strchr(name, '=');
    if (value != NULL) {
        *value = '\0';
        value++;
        if (setenv(name, value, 1) == -1) {
            perror("setenv");
        } else {
            printf("Environment variable set: %s=%s\n", name, value);
        }
    } else {
        fprintf(stderr, "Invalid environment variable format: %s\n", name_value);
    }
    free(name);
}

int main(int argc, char *argv[]) {
    int opt;
    opterr = 0;

    while ((opt = getopt(argc, argv, "ispucC:dU:vV:")) != -1) {
        switch (opt) {
            case 'i':
                print_ids();
                break;
            case 's':
                become_group_leader();
                break;
            case 'p':
                print_pids();
                break;
            case 'u':
                print_ulimit();
                break;
            case 'U':
                set_ulimit(optarg);
                break;
            case 'c':
                print_core_size();
                break;
            case 'C':
                set_core_size(optarg);
                break;
            case 'd':
                print_cwd();
                break;
            case 'v':
                print_env();
                break;
            case 'V':
                set_env(optarg);
                break;
            case '?':
                fprintf(stderr, "Unknown option: %c\n", optopt);
                break;
            default:
                fprintf(stderr, "Usage: %s [-i] [-s] [-p] [-u] [-U new_ulimit] [-c] [-C size] [-d] [-v] [-V name=value]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    return 0;
}
