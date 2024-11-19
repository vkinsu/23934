#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

int error_check(const char *file_path, struct stat *file_stat) {
    if (stat(file_path, file_stat) == -1) {
        perror(file_path);
        return 0;
    }
    return 1;
}


void print_file_info(const char *file_path) {
    struct stat file_stat;

    if (!error_check(file_path, &file_stat))
        return;

    // File type

    char file_type = '?';
    if (S_ISDIR(file_stat.st_mode)) {
        file_type = 'd';
    } else if (S_ISREG(file_stat.st_mode)) {
        file_type = '-';
    }

    // Permissions
    char permissions[10] = "---------";
    permissions[0] = (file_stat.st_mode & S_IRUSR) ? 'r' : '-';
    permissions[1] = (file_stat.st_mode & S_IWUSR) ? 'w' : '-';
    permissions[2] = (file_stat.st_mode & S_IXUSR) ? 'x' : '-';
    permissions[3] = (file_stat.st_mode & S_IRGRP) ? 'r' : '-';
    permissions[4] = (file_stat.st_mode & S_IWGRP) ? 'w' : '-';
    permissions[5] = (file_stat.st_mode & S_IXGRP) ? 'x' : '-';
    permissions[6] = (file_stat.st_mode & S_IROTH) ? 'r' : '-';
    permissions[7] = (file_stat.st_mode & S_IWOTH) ? 'w' : '-';
    permissions[8] = (file_stat.st_mode & S_IXOTH) ? 'x' : '-';

    // Number of links
    int nlink = file_stat.st_nlink;

    // Owner and group
    struct passwd *pw = getpwuid(file_stat.st_uid);
    struct group *gr = getgrgid(file_stat.st_gid);
    const char *owner = (pw) ? pw->pw_name : "unknown";
    const char *group = (gr) ? gr->gr_name : "unknown";

    // Size
    char size_str[16];
    snprintf(size_str, sizeof(size_str), "%ld", file_stat.st_size);

    // Modification time
    char modified_time[20];
    strftime(modified_time, sizeof(modified_time), "%Y-%m-%d %H:%M", localtime(&file_stat.st_mtime));

    // File name
    const char *file_name = strrchr(file_path, '/');
    file_name = (file_name) ? file_name + 1 : file_path;

    // Print the information
    printf("%c%s %3d %-8s %-8s %8s %s %s\n",
           file_type, permissions, nlink, owner, group, size_str, modified_time, file_name);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> [<file2> ...]\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        print_file_info(argv[i]);
    }

    return 0;
}

