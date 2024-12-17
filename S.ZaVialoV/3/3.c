#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
    uid_t real = getuid();
    uid_t effective = geteuid();
    
    printf("real: %u\n", real);
    printf("effective: %u\n", effective);
    
    FILE* file = fopen("test3.txt", "r");
    if(file == NULL){
        perror("fopen");
        exit(0);
    }
    else
        fclose(file);
    
    setuid(real);
    effective = geteuid();
    printf("real: %u\n", real);
    printf("effective: %u\n", effective);
    
    file = fopen("test3.txt", "r");
    if(file == NULL){
        perror("fopen");
        exit(0);
    }
    else
        fclose(file);
    
    return 0;
}
