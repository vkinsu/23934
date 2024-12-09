#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

void print_file_info(const char *filename) {
    struct stat fileStat;
    if (lstat(filename, &fileStat) < 0) {
        perror("lstat");
        return;
    }

    // Определяем тип файла
    char fileType = '?';
    if (S_ISDIR(fileStat.st_mode)) {
        fileType = 'd';
    } else if (S_ISREG(fileStat.st_mode)) {
        fileType = '-';
    } else if (S_ISLNK(fileStat.st_mode)) {
        fileType = 'l';
    } else if (S_ISCHR(fileStat.st_mode)) {
        fileType = 'c';
    } else if (S_ISBLK(fileStat.st_mode)) {
        fileType = 'b';
    } else if (S_ISFIFO(fileStat.st_mode)) {
        fileType = 'p';
    } else if (S_ISSOCK(fileStat.st_mode)) {
        fileType = 's';
    }

    // Определяем права доступа
    char permissions[10] = "---------";
    if (fileStat.st_mode & S_IRUSR) permissions[0] = 'r';
    if (fileStat.st_mode & S_IWUSR) permissions[1] = 'w';
    if (fileStat.st_mode & S_IXUSR) permissions[2] = 'x';
    if (fileStat.st_mode & S_IRGRP) permissions[3] = 'r';
    if (fileStat.st_mode & S_IWGRP) permissions[4] = 'w';
    if (fileStat.st_mode & S_IXGRP) permissions[5] = 'x';
    if (fileStat.st_mode & S_IROTH) permissions[6] = 'r';
    if (fileStat.st_mode & S_IWOTH) permissions[7] = 'w';
    if (fileStat.st_mode & S_IXOTH) permissions[8] = 'x';

    // Получаем информацию о владельце и группе
    struct passwd *pwd = getpwuid(fileStat.st_uid);
    struct group *grp = getgrgid(fileStat.st_gid);

    // Получаем время последней модификации
    char timeStr[20];
    struct tm *timeInfo = localtime(&fileStat.st_mtime);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M", timeInfo);

    // Получаем имя файла
    char *fileName = strrchr(filename, '/');
    if (fileName) {
        fileName++;
    } else {
        fileName = (char *)filename;
    }

    // Выводим информацию о файле
    printf("%c%s %3lu %s %s %9lu %s %s\n",
           fileType, permissions,
           (unsigned long)fileStat.st_nlink,
           pwd ? pwd->pw_name : "unknown",
           grp ? grp->gr_name : "unknown",
           (unsigned long)fileStat.st_size,
           timeStr, fileName);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> [<file2> ...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        print_file_info(argv[i]);
    }

    return EXIT_SUCCESS;
}

