#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 40
#define CTRL_G '\007'
#define CTRL_D '\004' 
#define CTRL_W '\027' 
#define ERASE    '\b' 
#define KILL     '\x0C' 

// Функция для настройки терминала
void setInputMode() {
    struct termios terminal;
    tcgetattr(STDIN_FILENO, &terminal);
    terminal.c_lflag &= ~(ICANON | ECHO); // Отключаем канонический режим и эхо
    tcsetattr(STDIN_FILENO, TCSANOW, &terminal);
}

// Функция для печати строки с учетом текущей позиции
void printBuffer(const char *buffer, int start_index, int current_position) {
    // Убедимся, что start_index не выходит за пределы буфера
    if (start_index < 0) {
        start_index = 0;
    }

    int length_to_print = current_position - start_index;
    
    // Убедимся, что длина не выходит за пределы буфера
    if (length_to_print < 0) {
        length_to_print = 0;
    }

    printf("\r> %.*s", length_to_print, buffer + start_index);
    fflush(stdout);

    for (int i = 0; i < length_to_print; i++) {
        printf("\b \b");
    }
}

int main() {
    setInputMode();

    char *input = malloc(1024);

    input[0] = '\0'; 
    int total_length = 0; 
    int position = 0; 
    
    printf("Введите текст (CTRL-D для выхода):\n");

    while (1) {
        int str_num = total_length/MAX_LENGTH;
        char c;
        read(STDIN_FILENO, &c, 1); 

        if (c == CTRL_D && position == 0) {break;}
        if (c == CTRL_W && total_length > 0) {
            // Удалить последнее слово

            while (total_length > 0 && input[total_length - 1] == ' ') {
                total_length--;
                input[total_length] = '\0';
            }
            while (total_length > 0 && input[total_length - 1] != ' ') {
                total_length--;
                input[total_length] = '\0';
            }
            while (total_length > 0 && input[total_length - 1] == ' ') {
                total_length--;
                input[total_length] = '\0';
            }
            str_num = total_length/MAX_LENGTH;
            position = total_length % MAX_LENGTH;
            printBuffer(input, total_length - position + str_num, total_length);
            continue;
        }

        if (c == KILL && total_length > 0) {
            // Очистить текущую строку
            for (int i = total_length; i % MAX_LENGTH >= 0; i--) {input[i] = '\0';}
            total_length -= position;
            position = MAX_LENGTH;
            printBuffer(input, total_length - (total_length % MAX_LENGTH) + str_num, total_length);
            continue;
        }

        if (c == ERASE && total_length > 0) {
            total_length--;
            input[total_length] = '\0';
            position = total_length % MAX_LENGTH;
            printBuffer(input, total_length - position + str_num, total_length);
            continue;
        }

        // Обработка обычного текста
        if (total_length < 1024 && c > 30 && c < 127) {
            if (position > MAX_LENGTH) {
                write(STDOUT_FILENO, "\n", 1);
                position = 0;
                printBuffer(input, total_length - position, total_length);
            }
            input[total_length++] = c;
            position++;
            printBuffer(input, total_length - position, total_length);
        } else {
            // Если длина превышает 1024, игнорируем дополнительные символы
            write(STDOUT_FILENO, (const char[]){CTRL_G}, 1);
            continue;
        }
    }

    printf("\nЗавершено.\n");
    free(input);
    return 0;
}