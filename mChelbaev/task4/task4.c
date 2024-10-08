#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ORDERLIST
{
    char* str;
    struct ORDERLIST* pointer;
}ORDERLIST;

ORDERLIST* init (){
    ORDERLIST* end = (ORDERLIST*)malloc(1 * sizeof(ORDERLIST));
    ORDERLIST* start = (ORDERLIST*)malloc(1 * sizeof(ORDERLIST));
    end->pointer = 0;
    start->pointer = end;
    end->str = NULL;
    start->str = 0;
    return start;
}

ORDERLIST* append (ORDERLIST* current){
    char string[1<<10];
    int len = 1 << 10;
    fgets(string, len, stdin);
    if (string[0] == '.'){
        current->pointer->str = (char*)1;
        return current;
    }
    current->str = (char*)malloc(strlen(string + 1) * sizeof(char));
    strcpy(current->str, string);
    ORDERLIST* new = (ORDERLIST*)malloc(1 * sizeof(ORDERLIST));
    new->pointer = current->pointer;
    current->pointer = new;
    return new;
}

void list_delete(ORDERLIST* current){
    if (current->str != (char*)1){
        printf(current->str);
        free(current->str);
        ORDERLIST* pointer = current->pointer;
        return list_delete(pointer);
    }
}

int main(){
    ORDERLIST first = *init();
    ORDERLIST* current = &first;
    while (current->pointer->str == 0){
        current = append(current);
    }
    current = &first;
    printf("!!!Your output!!!\n");
    list_delete(current);
}
