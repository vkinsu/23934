#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS

typedef struct List {
    char* string;
    int next;
} List;

int main() {
    int current_size = 10;
    char buffer[100];
    List* str_list = (List*)malloc(sizeof(List) * current_size);
    int count = 0, flag = 0;

    while (flag == 0) {
        printf("Введите строку: ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            break;
        }
        buffer[strcspn(buffer, "\n")] = 0;
        str_list[count].string = (char*)malloc(strlen(buffer) + 1);
        strcpy(str_list[count].string, buffer);

        if (buffer[strlen(buffer) - 1] == '.') {
            flag = 1;
        }

        if (flag == 0){
            str_list[count].next = count + 1;
        }
        count++;

        if (count >= current_size) {
            current_size *= 2;
            str_list = (List*)realloc(str_list, sizeof(List) * current_size);
        }
    }
    str_list[count - 1].string[strlen(buffer) - 1] = 0;

    for (int i = 0; i < count; i++){
        printf("%s\n", str_list[i].string);
        free(str_list[i].string);
    }

    free(str_list); 
    return 0;
}