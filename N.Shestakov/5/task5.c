#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MAX_LINES 1000
#define MAX_LINE_LENGTH 256

typedef struct LineInfo {
    int position;
    int tabs;
    size_t length;
}LineInfo;

int main() {
    int fd = open("datafile.vi", O_RDONLY);

    LineInfo lines[MAX_LINES];
    char buffer[MAX_LINE_LENGTH];
    size_t line_count = 0;

    for (int i = 0; i < MAX_LINES; i++) {
        lines[i].position = 0;
        lines[i].length = 0;
        lines[i].tabs = 0;
    }

    int pos = 0, flag = 0, tab = 1;
    while (read(fd, buffer, 1) > 0 && line_count < MAX_LINES) {
        if (tab == 1 && (buffer[0] != ' ' || buffer[0] != ' ')){
            tab = 0;
        }
        else if (tab == 1){
            lines[line_count].tabs++;
        }

        if (buffer[0] == '\n' || buffer[0] == '\0') {
            lines[line_count+1].position = pos + 1;
            line_count++;
            flag = 0;
            tab = 1;
        }
        else if (tab == 0) {
            flag = 1;
            lines[line_count].length++;
        }
        pos++;
    }


    for (int i = 0; i <= line_count; i++){
        printf("%d\n", lines[i].tabs);
    }


    char line_number;
    char enter;

    while (1) {
        printf("Введите номер строки(0 для выхода): ");
        scanf("%c", &line_number);
	scanf("%c", &enter);

        if (line_number == '0') {
            break;
        }
	line_number -= 48;
        if (line_number > 0 && line_number <= line_count + 1) {
            lseek(fd, lines[line_number - 1].position + lines[line_number - 1].tabs, SEEK_SET);
            read(fd, buffer, lines[line_number - 1].length);
            buffer[lines[line_number - 1].length] = 0;

            printf("Строка %zu: %s\n", line_number, buffer);

            for (int i = 0; i < lines[line_number - 1].length; i++){
                buffer[i] = 0;
            }
        }
        else {
            printf("Неверный номер строки\n");
        }
    }

    close(fd);
    return 0;
}
