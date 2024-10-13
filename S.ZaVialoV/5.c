#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main(){
    int in = open("test5.txt", O_RDONLY);
    
    int n = 1;
    long long* arr = malloc(1000 * sizeof(long long));
    arr[0] = 0;

    long long i=0;
    char c;
    
    while(read(in, &c, 1)){
        if(c == '\n') arr[n++] = i+1;
        i++;
    }
    arr[n++] = i-1;
    close(in);
    
    int num;
    printf("Enter the line number.\n");
    scanf("%d", &num);
    
    if(num > 0 && num <= n){
        in = open("test5.txt", O_RDONLY);
        lseek(in, arr[num-1], 0);
        read(in, &c, 1);
        
        while(c != '\n' && c != 0){
            printf("%c", c);
            read(in, &c, 1);
        }
        
        close(in);
    }
    free(arr);
    return 0;
}
