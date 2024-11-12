#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>

// typedef struct stat stat;

void print_file_info(const char *file_path)
{
    struct stat file_stat;

    if (lstat(file_path, &file_stat) == -1)
    {
        perror("err\n");
        return;
    }

    char type = 0;

    if (S_ISDIR(file_stat.st_mode))
    {
        type = 'd';
    }
    else if (S_ISREG(file_stat.st_mode))
    {
        type = '-';
    }
    else
    {
        type = '?';
    }

    // Определяем права доступа
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
    int nlink = file_stat.st_nlink;

    // Имя владельца
    struct passwd *pwd = getpwuid(file_stat.st_uid); // по идентефиктору пользователя
    char *owner = pwd ? pwd->pw_name : (char *)"unknown";

    // Имя группы
    struct group *grp = getgrgid(file_stat.st_gid);
    char *group = grp ? grp->gr_name : (char *)"unknown";

    // Размер файла (только если это обычный файл)
    char size_str[16];
    if (S_ISREG(file_stat.st_mode))
    {
        snprintf(size_str, sizeof(size_str), "%ld", file_stat.st_size);
    }
    else
    {
        size_str[0] = '\0';
    }

    // Дата и время последней модификации
    char time_str[20];
    struct tm *mtime = localtime(&file_stat.st_mtime);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M", mtime);

    // Имя файла (без пути)
    char *file_name = basename((char *)file_path);

    // Вывод информации о файле в формате таблицы
    printf("%c%s %3d %-8s %-8s %8s %s %s\n",
           type,        // Тип файла
           permissions, // Права доступа
           nlink,       // Количество ссылок
           owner,       // Имя владельца
           group,       // Имя группы
           size_str,    // Размер файла (если обычный)
           time_str,    // Время последней модификации
           file_name    // Имя файла
    );
}

int main(int argc, char *argv[])
{
    if (argc < 2)
        return 1;

    for (int i = 1; i < argc; i++)
    {
        print_file_info(argv[i]);
    }

    return 0;
}