#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define ERASE write(1, "\b \b", 4) // Macro to erase the last character from the terminal

int main() {
    struct termios settings; // Terminal settings
    struct termios saved_settings; // Saved terminal settings
    tcgetattr(0, &settings); // Get current terminal settings
    memcpy(&saved_settings, &settings, sizeof(saved_settings)); // Save a copy of the settings
    settings.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    settings.c_cc[VMIN] = 1; // Minimum number of characters to read
    settings.c_cc[VTIME] = 0; // Timeout for read operations
    tcsetattr(0, TCSANOW, &settings); // Apply the new settings

    char line[41]; // Buffer to store input
    int pos = 0; // Current cursor position
    while (read(0, &line[pos], 1) == 1) { // Read one character from stdin
        if (pos > 0 && line[pos] == settings.c_cc[VERASE]) { // If backspace is pressed, erase the last character
            ERASE;
            --pos;
        } else if (line[pos] == settings.c_cc[VKILL]) { // If kill is pressed, erase the entire line
            while (pos > 0) {
                ERASE;
                --pos;
            }
            tcflush(0, TCIFLUSH); // Flush the input buffer
            tcsetattr(0, TCSANOW, &settings); // Restore terminal settings
        } else if (line[pos] == settings.c_cc[VWERASE]) { // If Ctrl+W is pressed, erase the last word
            while (pos > 0 && !isspace(line[pos - 1])) {
                ERASE;
                --pos;
            }
            while (pos > 0 && isspace(line[pos - 1])) {
                ERASE;
                --pos;
            }
        } else if (pos == 0 && line[pos] == 4) { // If Ctrl+D is pressed and the line is empty, exit the program
            tcsetattr(0, TCSANOW, &saved_settings);
            return 0;
        } else if (!isprint(line[pos])) { // If the character is not printable, emit a beep
            write(1, "\a", 1);
        } else if (pos == 40) { // If the line reaches the limit, wrap to a new line
            while (pos > 0 && !isspace(line[pos - 1])) {
                ERASE;
                --pos;
            }
            write(1, "\n", 1);
            int k = 0;
            int i;
            for (i = pos; i < 40; ++i) {
                write(1, &line[i], 1);
                ++k;
            }
            pos = k;
        } else { // Write the character to the terminal
            write(1, &line[pos], 1);
            ++pos;
        }
    }
    tcsetattr(0, TCSANOW, &saved_settings); // Restore terminal settings before exiting
    return 0;
}
