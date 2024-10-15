#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char line[500];
    char **list = NULL;
    int count = 0;

    while (1) {

        fgets(line, sizeof(line), stdin);
        if (line[0] == '.') {
            break;
        }
        line[strcspn(line, "\n")] = ' ';

        char *new_line = (char *)malloc(strlen(line) + 1);
        strcpy(new_line, line);

        char **temp = realloc(list, (count + 1) * sizeof(char *));

        list = temp;
        list[count] = new_line;
        count++;
    }

    for (int i = 0; i < count; i++) {
        printf("%s\n", list[i]);
        free(list[i]);
    }

    free(list);

    return 0;
}
