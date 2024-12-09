#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

void print_file_info(const char *filename) {
    struct stat fileStat;

    // Получаем статус файловой системы
    if (stat(filename, &fileStat) < 0) {
        perror(filename);
        return;
    }

    // Тип файла
    char fileType = '?';
    if (S_ISDIR(fileStat.st_mode)) {
        fileType = 'd';
    } else if (S_ISREG(fileStat.st_mode)) {
        fileType = '-';
    } else if (S_ISLNK(fileStat.st_mode)) {
        fileType = 'l';
    }

    // Права доступа
    char permissions[11];
    permissions[0] = fileType; // тип файла
    permissions[1] = (fileStat.st_mode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (fileStat.st_mode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (fileStat.st_mode & S_IXUSR) ? 'x' : '-';
    permissions[4] = (fileStat.st_mode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (fileStat.st_mode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (fileStat.st_mode & S_IXGRP) ? 'x' : '-';
    permissions[7] = (fileStat.st_mode & S_IROTH) ? 'r' : '-';
    permissions[8] = (fileStat.st_mode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (fileStat.st_mode & S_IXOTH) ? 'x' : '-';
    permissions[10] = '\0';

    // Число жестких ссылок
    int linkCount = fileStat.st_nlink;

    // Владелец и группа
    struct passwd *pw = getpwuid(fileStat.st_uid);
    struct group  *gr = getgrgid(fileStat.st_gid);
    char *ownerName = pw ? pw->pw_name : "unknown";
    char *groupName = gr ? gr->gr_name : "unknown";

    // Размер файла
    off_t fileSize = fileStat.st_size;

    // Время последнего изменения
    char timeStr[100];
    struct tm *timeInfo = localtime(&fileStat.st_mtime);
    strftime(timeStr, sizeof(timeStr), "%b %d %H:%M", timeInfo);

    // Вывод информации о файле
    printf("%c\n %s\n %s\n %s\n %lld\n %s\n %s\n", fileType, permissions, ownerName, groupName, (long long)fileSize, timeStr, filename);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s <имя_файла_или_каталога>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Обрабатываем все аргументы командной строки
    int i;
    for (i = 1; i < argc; i++) {
        print_file_info(argv[i]);
    }

    return EXIT_SUCCESS;
}
