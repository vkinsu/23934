#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void open_file(){
    FILE *file = fopen("datafile.txt", "w+");
    if (file == NULL) {
        perror("Не удалось открыть файл\n");
    } else {
        printf("Файл успешно открыт.\n");
        fclose(file);
    }
}

int main() {
    uid_t real_uid = getuid();
    uid_t effective_uid = geteuid();

    open_file();

    printf("Реальный идентификатор пользователя: %d\n", real_uid);
    printf("Эффективный идентификатор пользователя: %d\n", effective_uid);

    if (setuid(real_uid) != 0) {
        perror("Ошибка при установке идентификатора пользователя\n");
    }

    effective_uid = geteuid();
    printf("Реальный идентификатор: %d\n", real_uid);
    printf("Эффективный идентификатор: %d\n", effective_uid);

    open_file();
    return 0;
}