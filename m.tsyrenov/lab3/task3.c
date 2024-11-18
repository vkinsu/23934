#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() 
{
    uid_t real_uid = getuid();
    uid_t effective_uid =geteuid();
    printf("Real¹ UID: %d\n", real_uid);
    printf("Effective  UID: %d\n", effective_uid);

    if (setuid(real_uid) == -1) 
    {
        perror("faiiled to install to effective UID");
        exit(EXIT_FAILURE);
    }
    effective_uid = geteuid();
    printf("Afterµ setuid,effectiveÐ¹ UID: %d\n", effective_uid);
    FILE *file = fopen("test.txt", "r");
    if (file == NULL) 
    {
        perror("The file not could be opened¹Ð»");
        return EXIT_FAILURE;
    }
    fclose(file);
    printf("The file has been sucdesfully opened and closed‚.\n");
    return EXIT_SUCCESS;
}
