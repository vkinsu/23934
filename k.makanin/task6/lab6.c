#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

typedef struct line {
    off_t offset;
    off_t len;
} line;

int initLine(line **lines) {
    *lines = (line*) malloc(sizeof(line));
    if (*lines == NULL) {
        perror("Failed to create malloc.");
        return -1;
    }
    return 1;
}

int addMemory(int *extension, int numberOfLine, line **lines) {
    if ((*extension) <= numberOfLine) {
        (*extension) *= 2;
        *lines = realloc(*lines, (*extension) * sizeof(line));
        if (*lines == NULL) {
            perror("Failed to create realloc.");
            return -1;
        }
    }
    return 1;
}

int printLine(line *lines, int fd, int choice) {
    off_t lenStr = lines[choice - 1].len, specialOff = lines[choice - 1].offset, read_bytes = 1;
    char strOut[BUFSIZ];
    while (read_bytes > 0) {
        if (lseek(fd, specialOff, SEEK_SET) == -1) {
            perror("Failed to execute lseek.");
            return -1;
        }
        if (lenStr > BUFSIZ) {
            if ((read_bytes = read(fd, strOut, BUFSIZ)) == -1) {
                perror("Failed to read.");
                return -1;
            }
        } else {
            if ((read_bytes = read(fd, strOut, lenStr)) == -1) {
                perror("Failed to read.");
                return -1;
            }
        }

        if (fwrite(strOut, 1, read_bytes, stdout) == -1) {
            perror("Failed to fwrite.");
            return -1;
        }
        specialOff += read_bytes;
        lenStr -= read_bytes;
    }
    return 1;
}

void printStruct(line *lines, int num) {
    for (int i = 0; i < num; i++) {
        printf("%d - offset: %ld len: %ld \n", i, lines[i].offset, lines[i].len);
    }
}

void printFile(int fd) {
    char buf[BUFSIZ];
    int read_bytes;
    while ((read_bytes = read(fd, buf, BUFSIZ)) > 0) {
        if (fwrite(buf, 1, read_bytes, stdout) == -1) {
            perror("Failed to fwrite.");
            return;
        }
    }
    if (read_bytes == -1) {
        perror("Failed to read.");
    }
}

void alarm_handler(int sig) {
    // Do nothing, just catch the signal
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "No filename.\n");
        return -1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Failed to open.");
        return -1;
    }

    off_t lenStr = 0,
        previousLen = 0, 
        current = 0, 
        amountBytes;

    int extension = 1, numberOfLine = 0, scan_out; 

    char buf[BUFSIZ];

    line *lines;
    if (initLine(&lines) == -1){
        return -1;
    }

    if ((amountBytes = read(fd, buf, BUFSIZ)) == -1) {
        perror("Failed to read.");
        return -1;
    }

    while (amountBytes > 0){
        if (buf[current] == '\n'){
            if (addMemory(&extension, numberOfLine, &lines) == -1){
                return -1;
            }
            lines[numberOfLine].offset = previousLen;
            lines[numberOfLine].len = lenStr + 1;
            previousLen += lenStr + 1;
            lenStr = 0;
            numberOfLine++;
            amountBytes--;
            current++;
        } else {
            lenStr++;
            current++;
            amountBytes--;
        }
        if (amountBytes == 0){
            if ((amountBytes = read(fd, buf, BUFSIZ)) == -1) {
                perror("Failed to read.");
                return -1;
            }
            current = 0;
        }

    }

    lines[numberOfLine].offset = previousLen;
    lines[numberOfLine].len = lenStr;
    numberOfLine++;

    char choice_buffer[20];
    long choice = 1;

    printStruct(lines, numberOfLine);

    // Set up signal handler for SIGALRM
    struct sigaction sa;
    sa.sa_handler = alarm_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGALRM, &sa, NULL);

    while (choice != 0) {
        // Set alarm for 5 seconds
        alarm(5);

        printf("Write number of line: ");
        scan_out = scanf("%20s", choice_buffer);

        // Cancel alarm if input is received
        alarm(0);

        if (scan_out == EOF) {
            printf("Timeout. Printing entire file.\n");
            printFile(fd);
            free(lines);
            close(fd);
            return 0;
        }

        while (((choice = atol(choice_buffer)) == 0 && choice_buffer[0] != '0') || choice < 0) {
            printf("Incorrect number.\nTry again: ");
            scanf("%20s", choice_buffer);
        }
        if (choice_buffer[0] == '0') {
            break;
        }
        
        if (choice > numberOfLine) {
            printf("Incorrect number.\nTry again: ");
        } else {
            printLine(lines, fd, choice);
        }
    }

    free(lines);
    close(fd);

    return 0;
}