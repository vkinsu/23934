#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() 
{
    uid_t real_uid = getuid();
    uid_t effective_uid =geteuid();
    printf("Реальный UID: %d\n", real_uid);
    printf("Эффективный UID: %d\n", effective_uid);

    if (setuid(real_uid) == -1) 
    {
        perror("Не удалось установить эффективный UID");
        exit(EXIT_FAILURE);
    }
    effective_uid = geteuid();
    printf("После setuid, эффективный UID: %d\n", effective_uid);
    FILE *file = fopen("test.txt", "r");
    if (file == NULL) 
    {
        perror("Не удалось открыть файл");
        return EXIT_FAILURE;
    }
    fclose(file);
    printf("Файл успешно открыт и закрыт.\n");
    return EXIT_SUCCESS;
}
