#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <malloc.h>
#include <unistd.h>

typedef struct PAIR {
    int shift;
    int size;
}PAIR;

typedef struct  PAIR_ARRAY {
    int size;
    int last;
    PAIR* array;
}PAIR_ARRAY;

void push_back(PAIR_ARRAY* pointer, PAIR pair){
    if (pointer->last == pointer->size){
        pointer->size = (pointer->size == 0 ? 1 : pointer->size * 2);
        if (pointer->size == 1)
            pointer->array = (PAIR*) malloc (pointer->size * sizeof(PAIR));
        else     
            pointer->array = (PAIR*)realloc(pointer->array, pointer->size * sizeof(PAIR));
    }
    pointer->array[pointer->last] = pair;
    pointer->last += 1;
}

void print_array (PAIR_ARRAY const table){
    for (int i = 0; i < table.last; i++)
        printf("%d %d\n", table.array[i].shift, table.array[i].size);
}

int main(){
    int fin = open("input.txt", O_RDONLY);
    PAIR_ARRAY array;
    PAIR current;
    array.size = 0;
    array.last = 0;
    current.shift = 0;
    char a, *string;
    int i = 0, r;
    do {
        r = read(fin, &a, 1);
        if ((a == '\n') || (r == 0)){
            current.size = i - current.shift;
            push_back(&array, current);
            current.shift = i + 1;
            current.size = 0;
        }
        i++;
    } while (r);
    
    while (1) {
        scanf("%d", &r);
        if (r == 0)
            break;
        if (r > array.last){
            perror("File hasn't got this string!\n");
            continue;
        }
        lseek(fin, array.array[r-1].shift, SEEK_SET);
        string = (char*)malloc(array.array[r-1].size + 1);
        string[0] = '\0';
	read(fin, string, array.array[r-1].size);
        printf("%s\n", string);
    }

    close(fin);
    return 0;
}
