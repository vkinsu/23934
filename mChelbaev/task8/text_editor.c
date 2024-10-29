#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int main(){
    int fout = open("output.txt", O_WRONLY | O_CREAT);
    if (fout == -1){
        perror("ERROR: file output.txt didn't open\n");
        exit(1);
    }
    
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    if(fcntl(fout, F_SETLK, &lock) == -1){
        perror("ERROR: file output.txt didn't lock\n");
        close(fout);
        return -1;
    }    

    printf("file output.txt lock\n");

    char *text = (char*)malloc(1000);
    printf("enter string\n");
    scanf("%s", text);
    text[sizeof(text)] = 0;
    if(write(fout, text, sizeof(text)) == -1){
        perror("ERROR: i can't enter this string in file");
        close(fout);
        return -1;
    }

    lock.l_type = F_UNLCK;
    if(fcntl(fout, F_SETLK, &lock) == -1){
        perror("ERROR: file output.txt didn't unlock\n");
        close(fout);
        exit(1);
    }

    printf("end\n");

    close(fout);
    return 0;
}
