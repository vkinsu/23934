#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>

void func(){
    printf("Time's up.\n");
    _exit(0);
}

int main(){
    int file, len;
    void *ptr;
    struct stat statbuf;
    long long *arr;
    
    file = open("test.txt", O_RDONLY);
    if(file == -1){
        perror("open");
        exit(0);
    }
    
    if(fstat(file, &statbuf) == -1){
        perror("fstat");
        exit(0);
    }
    
    arr = (long long*)malloc((statbuf.st_size+1) * sizeof(long long));
    len = 0;
    arr[len++] = 0;
    
    ptr = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, file, 0);
    if(ptr == MAP_FAILED){
        perror("mmap");
        exit(0);
    }
    
    for(long long i=0; *(char*)(ptr+i) != 0; i++)
        if(*(char*)(ptr+i) == '\n')
            arr[len++] = i+1;
    arr[--len] = 0;
    
    int num;
    signal(SIGALRM, func);
    printf("Enter the line number.\n");
    alarm(5);
    
    if(scanf("%d", &num) == 0){
        fprintf(stderr, "Wrong type.\n");
        exit(0);
    }
    
    if(num > 0 && num <= len){
        for(int i=arr[num-1]; *(char*)(ptr+i) != '\n' && *(char*)(ptr+i); i++)
            printf("%c", *(char*)(ptr+i));
    }
    else if(num == 0)
        printf("ok");
    else{
        fprintf(stderr, "Wrong index.\n");
        exit(0);
    }
    
    printf("\n");
    free(arr);
    return 0;
}

