#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/mman.h>

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

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("Ошибка получения информации о файле");
        close(fd);
        return EXIT_FAILURE;
    }

    char *content = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (content == MAP_FAILED) {
        perror("Ошибка отображения файла в память");
        close(fd);
        return EXIT_FAILURE;
    }
    close(fd);

    LineInfo lines[MAX_LINES];
    size_t line_count = 0;

    for (int i = 0; i < MAX_LINES; i++) {
        lines[i].position = 0;
        lines[i].length = 0;
        lines[i].tabs = 0;
    }

    int tab = 1;
    for (int i = 0; i < sb.st_size && line_count < MAX_LINES; i++){
        if (tab == 1 && (content[i] != ' ' || content[i] != '   ')){
            tab = 0;
        }
        else if(tab == 1){
            lines[line_count].tabs++;
        }

        if (content[i] == '\n' || content[i] == '\0'){
            lines[line_count+1].position = i + 1;
            line_count++;
            tab = 1;
        }
        else if (tab == 0){
            lines[line_count].length++;
        }
    }

    int n = 1000;
    char* line_number = (char*)malloc(sizeof(char) * n);
    for (int i = 0; i < n; i++){line_number[i] = '\0';}
    signal (SIGALRM, MyAlarm);

    while(1){
        alarm(5);
        printf("Введите номер строки(0 для выхода): ");
        scanf("%s", line_number);
        int number = s2i(line_number);
        if (number == 0) {
            break;
        }
        if (number > 0 && number <= line_count + 1){
            printf("Строка %d: ", number);
            int num = lines[number - 1].position + lines[number - 1].tabs;
            for (int i = 0; i < lines[number - 1].length; i++){
                printf("%c", content[num + i]);
            }
            printf("\n");
        }
        else {
            printf("Неверный номер строки\n");
        }
    }

    munmap(content, sb.st_size);
    return EXIT_SUCCESS;
}
