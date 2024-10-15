#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define SIZE 100

typedef struct List
{
    char *str;
    struct List *next;
} List;

void append(List *tail, char *tmp_str)
{
    tail->str = (char *)malloc(strlen(tmp_str) + 2);
    strcpy(tail->str, tmp_str);
    tail->next = (List *)0;
}

void print_list(List list)
{
    List *tail = &list;

    while (tail)
    {
        printf("%s", tail->str);
        tail = tail->next;
    }
}

int main()
{
    List list;
    List *tail = &list;

    char tmp_str[SIZE] = {'\0'};

    if (fgets(tmp_str, SIZE, stdin))
    {
        append(tail, tmp_str);
        int flag = tmp_str[strlen(tmp_str) - 2] != '.';

        while (flag && fgets(tmp_str, SIZE, stdin))
        {
            tail->next = (List *)malloc(sizeof(List));
            tail = tail->next;
            append(tail, tmp_str);

            if (tmp_str[strlen(tmp_str) - 2] == '.')
                flag = 0;
        }
    }

    print_list(list);

    return 0;
}