#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    int n = 0;
    char** list = malloc(0);
    char* buffer = malloc(1000);
    char b = 1;

    while(b){
        fgets(buffer, 999, stdin);
        if(buffer[0] != '.'){
            list = realloc(list, (n+1)*sizeof(char*));
            list[n] = malloc(strlen(buffer) + 2);
            strcpy(list[n++], buffer);
        }
        else b = 0;
    }

    free(buffer);
    for(int i=0; i<n; i++){
        printf("%s", list[i]);
        free(list[i]);
    }
    free(list);
    return 0;
}