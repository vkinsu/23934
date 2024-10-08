#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main (int argc, char** argv) {
    setuid(0);
    printf("real - %d, effective - %d\n", getuid(), geteuid());
    printf("Argv[1] %s\n", argv[1]);
    FILE* fin = fopen(argv[1], "r");
    if (fin){
        char* string = NULL;
        size_t len = 0;
        printf("before getline\n");
        getline(&string, &len, fin);
        printf("%s\n",string);
        fclose(fin);
    } else {
        perror("Error: i can open this file\n");
    }

    printf("\nI equated real and effective identifiers\n\n");

    setuid(geteuid());
    printf("real - %d, effective - %d\n", getuid(), geteuid());
    fin = fopen(argv[1], "r");
    if (fin){
        char* string = NULL;
        size_t len = 0;
        getline(&string, &len, fin);
        printf("%s\n",string);
        fclose(fin);
    } else {
        perror("Error: i can open this file\n");
        return 1;
    }
    printf("complite!\n");
    return 0;
}
