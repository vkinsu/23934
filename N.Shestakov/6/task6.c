#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define MAX_LINES 1000
#define MAX_LINE_LENGTH 256
#define INF 99999999
volatile sig_atomic_t alarm_triggered = 0;

typedef struct LineInfo {
    int position;
    int tabs;
    size_t length;
}LineInfo;

int my_pow(int number, int p){
    if (p == 0){return 1;}
    int result = number;
    for (int i = 0; i < p-1; i++){
        result *= number;
    }
    return result;
}

int s2i(char* str){
    int len = 0;
    while(str[len] != '\0'){
        if (str[len] > 47 && str[len] < 58)
            len++;
        else{
            return INF;
        }
    }
    if (str[0] == '0' && len > 1){return INF;}

    int result = 0;
    for (int i = 0, p = len-1; i < len; i++, p--){
        result += (str[i]-48) * my_pow(10, p);
    }

    return result;
}

void MyAlarm(int sign){
    puts("\nalarm is triggered!");
    _Exit(0);
}

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

    int n = 1000;
    char* line_number = (char*)malloc(sizeof(char) * n);
    for (int i = 0; i < n; i++){line_number[i] = '\0';}
    signal (SIGALRM, MyAlarm);

    while (1) {
        alarm(5);
        printf("Введите номер строки(0 для выхода): ");
        scanf("%s", line_number);
        int number = s2i(line_number);

        if (number == '0') {
            break;
        }
        if (number > 0 && number <= line_count + 1) {
            lseek(fd, lines[number - 1].position + lines[number - 1].tabs, SEEK_SET);
            read(fd, buffer, lines[number - 1].length);
            buffer[lines[number - 1].length] = 0;

            printf("Строка %d: %s\n", number, buffer);

            for (int i = 0; i < lines[number - 1].length; i++){
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
