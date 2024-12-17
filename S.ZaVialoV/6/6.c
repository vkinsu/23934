#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

void func(){
    printf("Time's up.\n");
    _exit(0);
}

int main(){
    int in = open("test.txt", O_RDONLY);
    if(in == -1){
        perror("test.txt (analysis)");
        exit(0);
    }
    
    char c;
    int n = 0;
    long long* arr = malloc(1000 * sizeof(long long));
    arr[n++] = 0;
    
    if(read(in, &c, 0) == -1){
        perror("read (analysis)");
        exit(0);
    }
    for(long long i=0; read(in, &c, 1); i++){
        if(c == '\n'){
            arr[n++] = i+1;
        }
    }
    arr[--n] = 0;
    
    if(close(in) == -1){
        perror("close (analysis)");
        exit(0);
    }
    
    int num;
    signal(SIGALRM, func);
    printf("Enter the line number.\n");
    alarm(5);
    
    if(scanf("%d", &num) == 0){
        fprintf(stderr, "Wrong type.\n");
        exit(0);
    }
    
    if(num > 0 && num <= n){
        in = open("test.txt", O_RDONLY);
        if(in == -1){
            perror("open (searching)");
            exit(0);
        }
        
        if(lseek(in, arr[num-1], 0) == -1){
            perror("lseek");
            exit(0);
        }
        
        if(read(in, &c, 0) == -1){
            perror("read (searching)");
            exit(0);
        }
        read(in, &c, 1);
        
        while(c != '\n' && c != 0){
            printf("%c", c);
            read(in, &c, 1);
        }
        printf("\n");
        
        if(close(in) == -1){
            perror("close");
            exit(0);
        }
    }
    else if(num != 0){
        fprintf(stderr, "Wrong index.\n");
        exit(0);
    }
    else
        printf("ok\n");
    free(arr);
    return 0;
}
