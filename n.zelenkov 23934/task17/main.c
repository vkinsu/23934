#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

#define MAX_LEN 40  // Максимальная длина строки
#define ERASE 127   // Код клавиши Backspace
#define KILL 21     // Клавиша CTRL+U для очистки строки
#define CTRL_W 23   // Клавиша CTRL+W для удаления слова
#define CTRL_D 4    // Клавиша CTRL+D для завершения
#define BELL 7      // Код для CTRL+G (звуковой сигнал)

void enable_raw_mode(struct termios *orig_termios) {
    struct termios raw;

    // Получаем текущие настройки терминала
    tcgetattr(STDIN_FILENO, orig_termios);
    raw = *orig_termios;

    // Отключаем канонический режим и эхо
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode(struct termios *orig_termios) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig_termios);
}

int main() {
    struct termios orig_termios;
    enable_raw_mode(&orig_termios);

    char line[MAX_LEN + 1] = {0};  // Хранение текущей строки
    int len = 0;  // Текущая длина строки

    while (1) {
        char c;
        read(STDIN_FILENO, &c, 1);

        if (c == CTRL_D && len == 0) {
            break;  // Завершение программы, если строка пуста и введен CTRL+D
        }
        else if (c == ERASE) {
            if (len > 0) {
                len--;  // Уменьшаем длину строки
                write(STDOUT_FILENO, "\b \b", 3);  // Удаление последнего символа с экрана
            }
        }
        else if (c == KILL) {
            while (len > 0) {
                len--;
                write(STDOUT_FILENO, "\b \b", 3);  // Удаление символов с экрана
            }
        }
        else if (c == CTRL_W) {
            // Удаляем последнее слово
            while (len > 0 && line[len - 1] == ' ') {
                len--;
                write(STDOUT_FILENO, "\b \b", 3);  // Удаление пробелов с экрана
            }
            while (len > 0 && line[len - 1] != ' ') {
                len--;
                write(STDOUT_FILENO, "\b \b", 3);  // Удаление символов слова с экрана
            }
        }
        else if (c >= 32 && c <= 126) {  // Печатаемые символы
            if (len < MAX_LEN) {
                line[len++] = c;
                write(STDOUT_FILENO, &c, 1);
            } else {
                write(STDOUT_FILENO, "\n", 1);  // Перенос строки, если длина превышена
                len = 0;
                line[len++] = c;
                write(STDOUT_FILENO, &c, 1);
            }
        }
        else {
            // Непечатаемый символ, кроме разрешенных
            write(STDOUT_FILENO, "\a", 1);  // Звуковой сигнал (CTRL-G)
        }
    }

    disable_raw_mode(&orig_termios);
    printf("\nПрограмма завершена.\n");

    return 0;
}

