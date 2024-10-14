#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main()
{
    printf("real user ID: %d, effective user ID: %d\n", getuid(), geteuid());

    FILE *file = fopen("test.txt", "r");
    if (file == NULL)
    {
        perror("Err\n");
        return 1;
    }

    fclose(file);

    if (setuid(getuid()) == -1)
    {
        perror("setuid failed");
        return 1;
    }

    printf("-----\nreal user ID: %d, effective user ID: %d\n", getuid(), geteuid());

    FILE *file2 = fopen("test.txt", "r");
    if (file2 == NULL)
    {
        perror("Err\n");
        return 1;
    }

    fclose(file2);

    if (setuid(getuid()) == -1)
    {
        perror("setuid failed");
        return 1;
    }

    return 0;
}
