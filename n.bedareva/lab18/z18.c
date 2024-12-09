#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

char tupy_file(struct stat *file_stat)
{
    char type;

    if (S_ISDIR(file_stat->st_mode)) {
        type = 'd';
    } else if (S_ISREG(file_stat->st_mode)) {
        type = '-';
    } else {
        type = '?';
    }
    return type;
}


void print_file_info(const char *filename) {
    struct stat file_stat;

    stat(filename, &file_stat);

    char type = tupy_file(&file_stat);

   
    char permissions[10];
    permissions[0] = type;
    permissions[1] = (file_stat.st_mode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (file_stat.st_mode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (file_stat.st_mode & S_IXUSR) ? 'x' : '-';
    permissions[4] = (file_stat.st_mode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (file_stat.st_mode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (file_stat.st_mode & S_IXGRP) ? 'x' : '-';
    permissions[7] = (file_stat.st_mode & S_IROTH) ? 'r' : '-';
    permissions[8] = (file_stat.st_mode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (file_stat.st_mode & S_IXOTH) ? 'x' : '-';
    permissions[10] = '0';

    
    nlink_t links = file_stat.st_nlink;

    
    struct passwd *pw = getpwuid(file_stat.st_uid);
    struct group  *gr = getgrgid(file_stat.st_gid);

    
    size_t size = 0;
    if (S_ISREG(file_stat.st_mode))
        size = file_stat.st_size;

    
    char time_str[20];
    struct tm *tm_info;
    tm_info = localtime(&file_stat.st_mtime);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M", tm_info);

  
    char *name = strrchr(filename, '/');
    if (name) {
        name++; 
    } else {
        name = filename; 
    }

   
    printf("%s %lu %s %s %10zu %s %s \n",permissions,links,pw->pw_name,gr->gr_name,size,time_str,name);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        print_file_info(argv[i]);
    }

    return 0;
}

