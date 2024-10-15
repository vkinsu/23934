#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

int main() {
    // Печать реального и эффективного идентификаторов пользователя
    uid_t real_uid = getuid();
    uid_t effective_uid = geteuid();
    
    printf("Real UID: %d\n", real_uid);
    printf("Effective UID: %d\n", effective_uid);

    // Открытие файла
    FILE *file = fopen("myfile.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
    } else {
        printf("File opened successfully.\n");
        fclose(file); // Закрытие файла
    }

    // Сделаем реальный и эффективный идентификаторы одинаковыми
    if (setuid(real_uid) == -1) {
        perror("setuid error");
        exit(EXIT_FAILURE);
    }

    // Повторяем печать идентификаторов
    printf("After setuid:\n");
    real_uid = getuid();
    effective_uid = geteuid();
    printf("Real UID: %d\n", real_uid);
    printf("Effective UID: %d\n", effective_uid);

    // Повторное открытие файла
    file = fopen("myfile.txt", "r");
    if (file == NULL) {
        perror("Error opening file after setuid");
    } else {
        printf("File opened successfully after setuid.\n");
        fclose(file);
    }

    return 0;
}

