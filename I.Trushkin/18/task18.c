#include<stdio.h>
#include <sys/stat.h>
#include<stdlib.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <string.h>


#define TABLESIZE (sizeof(table)/sizeof(*table))

enum regularFile  {
    unregular, regular
};
enum regularFile flag;


const char *red = "\x1b[31m";
const char *reset = "\x1b[0m";
const char *green = "\x1b[32m";
const char *purple = "\x1b[35m";



void viewingAccessRights(struct stat *stbuf) {
    char permissions[9] = "---------";
    if (stbuf->st_mode & S_IRUSR) permissions[0] = 'r';
    if (stbuf->st_mode & S_IWUSR) permissions[1] = 'w';
    if (stbuf->st_mode & S_IXUSR) permissions[2] = 'x';
    if (stbuf->st_mode & S_IRGRP) permissions[3] = 'r';
    if (stbuf->st_mode & S_IWGRP) permissions[4] = 'w';
    if (stbuf->st_mode & S_IXGRP) permissions[5] = 'x';
    if (stbuf->st_mode & S_IROTH) permissions[6] = 'r';
    if (stbuf->st_mode & S_IWOTH) permissions[7] = 'w';
    if (stbuf->st_mode & S_IXOTH) permissions[8] = 'x';

    printf("%s\t", permissions);
}

void viewingTypeOfFile (struct stat* stbuf) {
    switch(stbuf->st_mode & S_IFMT) {
        case S_IFDIR:
            printf("d");
            break;
        case S_IFREG:
            printf("-");
            flag = regular;
            break;
        default:
            printf("?");
        break;
    }
}

void viewingFileLinks(struct stat* stbuf) {
    struct passwd *pw;
    struct group *grp;

    pw = getpwuid(stbuf->st_uid);
    if (pw == NULL) {
        printf("%sError: сould not get owner information %s\n", red, reset);
    }
    else {
        printf("%s\t",pw->pw_name);
    }

    grp = getgrgid(stbuf->st_gid);
    if (grp == NULL) {
        printf("%sError: сould not get group information %s\n", red, reset);
    }
    else {
        printf("%s\t",grp->gr_name);
    }
}

void viewingTime(struct stat* stbuf) {
    struct tm *time = localtime(&stbuf->st_mtime);
    char strTime[40];
    strftime(strTime, sizeof(strTime), "%h %d %H:%M", time);
    printf("%s\t",strTime);
}
void viewingSizeFile(struct stat* stbuf) {
    if (flag == regular) {
        printf("%ld\t", stbuf->st_size);
    }
    else printf("\t");
}

void viewingFilename(char* filename) {
    char *chfilename = filename + strlen(filename) - 1;
    while (chfilename != filename && *chfilename != '/') {
        chfilename--;
    }
    if (*chfilename == '/') {
        chfilename++;
    }
    printf("%s \n", chfilename);
}

int main(int argc, char* argv[]) {

    if (argc == 1) {
        printf("%sError: no arguments entered %s\n", red, reset);
    }

    struct stat stbuf;

    if (stat(argv[1], &stbuf) == -1) {
        printf("%sError: Failed to get file information %s\n", red, reset);
        exit(1);
    }

    viewingTypeOfFile(&stbuf);
    viewingAccessRights(&stbuf);
    viewingFileLinks(&stbuf);
    viewingSizeFile(&stbuf);
    viewingTime(&stbuf);
    viewingFilename(argv[1]);
}