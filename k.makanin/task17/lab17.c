#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

#define MAX_LINE_LENGTH 40

void disable_echo_canonical_mode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void enable_echo_canonical_mode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void beep() {
    putchar('\a');
    fflush(stdout);
}

void erase_last_char(char *line, int *cursor) {
    if (*cursor > 0) {
        printf("\b \b");
        fflush(stdout);
        line[--*cursor] = '\0';
    }
}

void kill_line(char *line, int *cursor) {
    while (*cursor > 0) {
        printf("\b \b");
        fflush(stdout);
        line[--*cursor] = '\0';
    }
}

void erase_last_word(char *line, int *cursor) {
    while (*cursor > 0 && line[*cursor - 1] == ' ') {
        erase_last_char(line, cursor);
    }
    while (*cursor > 0 && line[*cursor - 1] != ' ') {
        erase_last_char(line, cursor);
    }
}

void handle_input(char *line, int *cursor) {
    char ch;
    while (read(STDIN_FILENO, &ch, 1) > 0) {
        if (ch == '\x7F') { // ERASE
            erase_last_char(line, cursor);
        } else if (ch == '\x15') { // KILL
            kill_line(line, cursor);
        } else if (ch == '\x17') { // CTRL-W
            erase_last_word(line, cursor);
        } else if (ch == '\x04' && *cursor == 0) { // CTRL-D at the beginning of the line
            break;
        } else if (ch == '\x07') { // Beep for unprintable characters
            beep();
        } else if (*cursor < MAX_LINE_LENGTH) {
            if (ch == '\n') {
                printf("\n");
                fflush(stdout);
                line[*cursor] = '\0';
                break;
            } else {
                putchar(ch);
                fflush(stdout);
                line[(*cursor)++] = ch;
                line[*cursor] = '\0';
            }
        } else {
            beep();
        }
    }
}

int main() {
    char line[MAX_LINE_LENGTH + 1] = {0};
    int cursor = 0;

    disable_echo_canonical_mode();

    handle_input(line, &cursor);

    enable_echo_canonical_mode();

    printf("\nFinal line: %s\n", line);

    return 0;
}