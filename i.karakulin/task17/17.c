#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 40
#define ERASE 127 // Код символа ERASE (обычно Backspace)
#define KILL 21   // Код символа KILL (обычно CTRL+U)
#define CTRL_W 23 // Код символа CTRL+W
#define CTRL_D 4  // Код символа CTRL+D
#define CTRL_G 7  // Код символа звукового сигнала

typedef struct termios termios;

void set_raw_mode(termios *original)
{
    termios raw = *original;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void reset_terminal_mode(termios *original)
{
    tcsetattr(STDIN_FILENO, TCSANOW, original);
}

void handle_erase(char *line, int *length)
{
    if (*length > 0)
    {
        (*length)--;
        printf("\b \b");
        fflush(stdout);
    }
}

void handle_kill(char *line, int *length)
{
    while (*length > 0)
    {
        (*length)--;
        printf("\b \b");
    }
    fflush(stdout);
}

void handle_ctrl_w(char *line, int *length)
{
    while (*length > 0 && isspace(line[*length - 1]))
    {
        (*length)--;
        printf("\b \b");
    }
    while (*length > 0 && !isspace(line[*length - 1]))
    {
        (*length)--;
        printf("\b \b");
    }
    fflush(stdout);
}

int main()
{
    termios original;
    tcgetattr(STDIN_FILENO, &original);

    set_raw_mode(&original);

    char line[MAX_LINE_LENGTH + 1];
    int length = 0;

    while (1)
    {
        int ch = getchar();

        if (ch == CTRL_D && length == 0)
        {
            break;
        }

        if (ch == ERASE)
        {
            handle_erase(line, &length);
        }
        else if (ch == KILL)
        {
            handle_kill(line, &length);
        }
        else if (ch == CTRL_W)
        {
            handle_ctrl_w(line, &length);
        }
        else if (isprint(ch))
        {
            if (length < MAX_LINE_LENGTH)
            {
                line[length++] = ch;
                putchar(ch);
            }
            else
            {
                putchar('\n');
                length = 0;
                line[length++] = ch;
                putchar(ch);
            }
            fflush(stdout); // отчистка потока
        }
        else
        {
            putchar(CTRL_G);
            fflush(stdout);
        }
    }

    reset_terminal_mode(&original);

    return 0;
}
