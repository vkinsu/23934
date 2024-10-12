#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ulimit.h>
#include <sys/types.h>
#include <sys/resource.h>

extern char **environ; //

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        perror("no arguments");
    }
    int option;
    char options[] = "ispuU:cC:dvV:";
    struct rlimit rlp;

    while ((option = getopt(argc, argv, options)) != -1) {
        switch (option) {
            case 'i' :
                uid_t real_uid = getuid();
                uid_t effective_uid = geteuid();

                gid_t real_gid = getgid();
                gid_t effective_gid = getegid();

                printf("Real UID: %u\n", real_uid);
                printf("Real EUID: %u\n", effective_uid);

                printf("Real GID: %u\n", real_gid);
                printf("Real EGID: %u\n", effective_gid);
                break;

            case 's' :
                if (setpgid(0, 0) == -1) {
                    perror("Failed to change process group ID.\n");
                } else {
                    printf("The process is the leader of the group.\n");
                }
                break;

            case 'p' :
                pid_t pid = getpid();
                pid_t ppid = getppid();
                pid_t pgid = getpgid(pid);

                printf("Current PID: %d\n", pid);
                printf("Current PPID: %d\n", ppid);
                printf("Current PGID: %d\n", pgid);

                break;

            case 'u' :

                if (getrlimit(RLIMIT_FSIZE, &rlp) == 0) {
                    printf("Ulimit value: %lu\n", rlp.rlim_cur);
                } else {
                    perror("Failed to get ulimit value\n");
                }
                break;

            case 'U' :
                int newUlimit = strtol(optarg, NULL, 10);
                if (newUlimit <= 0) {
                    perror("Size must be >0. \n");
                    break;
                }

                if (getrlimit(RLIMIT_FSIZE, &rlp) == 0) {
                    printf("Ulimit value: %lu\n", rlp.rlim_cur);
                } else {
                    perror("Failed to get ulimit value\n");
                    break;
                }
                rlp.rlim_cur = newUlimit;
                if (setrlimit(RLIMIT_FSIZE, &rlp) != 0) {
                    perror("Failed to set ulimit value\n");
                } else {
                    printf("Ulimit value is set\n");
                }
                break;

            case 'c' :

                if (getrlimit(RLIMIT_CORE, &rlp) == 0) {
                    printf("Size (in byte) of core-file: %lu\n", rlp.rlim_cur);
                } else {
                    perror("getrlimit");
                }
                break;

            case 'C' :

                if (getrlimit(RLIMIT_CORE, &rlp) == 0) {
                    printf("Size (in byte) of core-file: %lu\n", rlp.rlim_cur);
                } else {
                    perror("getrlimit");
                }

                int newSize = strtol(optarg, NULL, 10);
                if (newSize <= 0) {
                    perror("Size must be >0. \n");
                }

                rlp.rlim_cur = newSize;
                if (setrlimit(RLIMIT_CORE, &rlp) == -1) {
                    perror("Failed to resize core file.\n");
                } else {
                    printf("Core file size changed.\n");
                }
                break;

            case 'd' :
                char* cwd = getcwd(NULL, 0);
                if (cwd == NULL) {
                    perror("getcwd error");
                } else {
                    printf("Current working directory is: %s\n", cwd);
                    free(cwd);
                }
                break;

            case 'v' :
                char **p;
                printf("environment variables are:\n");
                for (p = environ; *p; p++) {
                    printf("%s\n", *p);
                }
                break;

            case 'V' :
                if (putenv(optarg) == 0) {
                    printf("Environment variable value is set.\n");
                } else {
                    perror("putenv");
                }
                break;
        }
    }
    return 0;
}
