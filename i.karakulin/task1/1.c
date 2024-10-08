#include <sys/resource.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>

extern char **environ;

void do_options(char opt)
{
    long tmp = 0;
    struct rlimit new_lim;

    switch (opt)
    {
    case 'i':
        printf("real user ID: %d, effective user ID: %d\nreal group ID: %d, effective group ID: %d", getuid(), geteuid(), getgid(), getegid());
        break;

    case 's':
        if (setpgid(getpid(), getpid()) == 0) // if PID = PGID, then procces with PID is lid group, меняет PGID
        {
            printf("successfully");
        }
        else
        {
            printf("error");
        }
        break;

    case 'p':
        printf("procces ID: %d, procces parent ID: %d, group ID: %d", getpid(), getppid(), getpgrp());
        break;

    case 'u':
        if (getrlimit(RLIMIT_FSIZE, &new_lim) == 0)
        {
            printf("file size soft lim: %ld, hard lim: %ld", new_lim.rlim_cur, new_lim.rlim_max);
        }
        else
        {
            printf("error");
        }
        break;

    case 'U':
        getrlimit(RLIMIT_FSIZE, &new_lim);
        tmp = atol(optarg);
        if (tmp < -1)
        {
            printf("error");
            break;
        }
        else
        {
            new_lim.rlim_cur = tmp;
        }

        if (-1 == setrlimit(RLIMIT_FSIZE, &new_lim))
        {
            printf("error\n");
        }
        else
        {
            getrlimit(RLIMIT_FSIZE, &new_lim);
            printf("new soft lim: %ld", new_lim.rlim_cur);
        }
        break;

    case 'c':
        if (-1 == getrlimit(RLIMIT_CORE, &new_lim))
        {
            printf("error\n");
        }
        else
        {
            printf("CORE file size soft lim: %ld, hard lim: %ld", new_lim.rlim_cur, new_lim.rlim_max);
        }
        break;

    case 'C':
        getrlimit(RLIMIT_CORE, &new_lim);
        new_lim.rlim_cur = atol(optarg);

        if (-1 == setrlimit(RLIMIT_CORE, &new_lim))
        {
            printf("error\n");
        }
        else
        {
            getrlimit(RLIMIT_CORE, &new_lim);
            printf("new soft lim CORE file size: %ld", new_lim.rlim_cur);
        }
        break;

    case 'd':
    {
        char *path = getenv("PWD");

        if (NULL == path)
        {
            printf("error\n");
        }
        else
        {
            printf("current directory: %s\n", path);
        }
        break;
    }

    case 'v':
    {
        char **ptr = environ;
        for (; *ptr != NULL; ptr++)
        {
            printf("%s\n", *ptr);
        }
        break;
    }

    case 'V':
    {
        if (-1 == putenv(optarg))
        {
            printf("error");
        }
        break;
    }

    case '?':
        printf("invalid option is %c", optopt);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    char options[] = "ispuU:cC:dvV:";
    char cur_opt = 0;
    char *opt = (char *)malloc(8);
    int ind = 0, size = 8;

    while ((cur_opt = (char)getopt(argc, argv, options)) != -1)
    {
        if (ind == size)
        {
            size *= 2;
            opt = (char *)realloc(opt, size);
            opt[ind] = cur_opt;
        }
        else
        {
            opt[ind] = cur_opt;
        }
        ++ind;
    }

    for (int i = ind - 1; i >= 0; --i)
    {
        do_options(opt[i]);
    }

    return 0;
}