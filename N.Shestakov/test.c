#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS

typedef struct List {
    char* string;
    int next;
}List;

int main() {
    int current_size = 10;
    char buffer[100] = { 0 };
    List* str_list = (List*)malloc(sizeof(List) * current_size);
    int count = 0;

    while (count < 1) {
        printf("Введите строку: ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {

            str_list[count].string = (char*)malloc(sizeof(char) * strlen(buffer));
            str_list[count].string = buffer;
            count++;
            if (current_size == count) {
                current_size *= 2;
                str_list = (List*)realloc(str_list, sizeof(List) * current_size);
            }
            if (count > 0){
                str_list[count - 1].next = count;
            }
        }
    }
    int len = 5;
    printf("%d", len);

    return 0;
}