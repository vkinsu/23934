#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>

void print_file_info(const char *path) {
    struct stat file_stat;
    if (stat(path, &file_stat) == -1) {
        perror("stat");
        return;
    }

    // Определение типа файла
    char file_type;
    if (S_ISDIR(file_stat.st_mode)) file_type = 'd';
    else if (S_ISREG(file_stat.st_mode)) file_type = '-';
    else file_type = '?';

    // Права доступа
    char permissions[10];
    permissions[0] = (file_stat.st_mode & S_IRUSR) ? 'r' : '-';
    permissions[1] = (file_stat.st_mode & S_IWUSR) ? 'w' : '-';
    permissions[2] = (file_stat.st_mode & S_IXUSR) ? 'x' : '-';
    permissions[3] = (file_stat.st_mode & S_IRGRP) ? 'r' : '-';
    permissions[4] = (file_stat.st_mode & S_IWGRP) ? 'w' : '-';
    permissions[5] = (file_stat.st_mode & S_IXGRP) ? 'x' : '-';
    permissions[6] = (file_stat.st_mode & S_IROTH) ? 'r' : '-';
    permissions[7] = (file_stat.st_mode & S_IWOTH) ? 'w' : '-';
    permissions[8] = (file_stat.st_mode & S_IXOTH) ? 'x' : '-';
    permissions[9] = '\0';

    // Количество ссылок
    int links = file_stat.st_nlink;

    // Получение имени владельца и группы
    struct passwd *pw = getpwuid(file_stat.st_uid);
    struct group *gr = getgrgid(file_stat.st_gid);
    const char *owner = pw ? pw->pw_name : "unknown";
    const char *group = gr ? gr->gr_name : "unknown";

    // Размер файла (только для обычных файлов)
    char size_str[16] = "";
    if (S_ISREG(file_stat.st_mode)) {
        snprintf(size_str, sizeof(size_str), "%ld", file_stat.st_size);
    }

    // Дата последней модификации
    char mod_time[20];
    struct tm *tm_info = localtime(&file_stat.st_mtime);
    strftime(mod_time, sizeof(mod_time), "%Y-%m-%d %H:%M", tm_info);

    // Имя файла
    char *filename = basename((char *)path);

    // Вывод информации в формате таблицы
    printf("%c%s %3d %-8s %-8s %8s %s %s\n", 
           file_type, permissions, links, owner, group, 
           size_str, mod_time, filename);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> <file2> ...\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        print_file_info(argv[i]);
    }

    return 0;
}