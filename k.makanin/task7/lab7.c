#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/mman.h>

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

int printLine(line *lines, char *file_data, int choice) {
    off_t lenStr = lines[choice - 1].len, specialOff = lines[choice - 1].offset;
    char *strOut = file_data + specialOff;
    if (fwrite(strOut, 1, lenStr, stdout) == -1) {
        perror("Failed to fwrite.");
        return -1;
    }
    return 1;
}

void printStruct(line *lines, int num) {
    for (int i = 0; i < num; i++) {
        printf("%d - offset: %ld len: %ld \n", i, lines[i].offset, lines[i].len);
    }
}

void printFile(char *file_data, off_t file_size) {
    if (fwrite(file_data, 1, file_size, stdout) == -1) {
        perror("Failed to fwrite.");
        return;
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

    struct stat file_stat;
    if (fstat(fd, &file_stat) == -1) {
        perror("Failed to fstat.");
        close(fd);
        return -1;
    }

    off_t file_size = file_stat.st_size;
    char *file_data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_data == MAP_FAILED) {
        perror("Failed to mmap.");
        close(fd);
        return -1;
    }

    off_t lenStr = 0,
        previousLen = 0, 
        current = 0;

    int extension = 1, numberOfLine = 0; 

    line *lines;
    if (initLine(&lines) == -1){
        munmap(file_data, file_size);
        close(fd);
        return -1;
    }

    while (current < file_size){
        if (file_data[current] == '\n'){
            if (addMemory(&extension, numberOfLine, &lines) == -1){
                munmap(file_data, file_size);
                close(fd);
                return -1;
            }
            lines[numberOfLine].offset = previousLen;
            lines[numberOfLine].len = lenStr + 1;
            previousLen += lenStr + 1;
            lenStr = 0;
            numberOfLine++;
            current++;
        } else {
            lenStr++;
            current++;
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
        int scan_out = scanf("%20s", choice_buffer);

        // Cancel alarm if input is received
        alarm(0);

        if (scan_out == EOF) {
            printf("Timeout. Printing entire file.\n");
            printFile(file_data, file_size);
            free(lines);
            munmap(file_data, file_size);
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
            printLine(lines, file_data, choice);
        }
    }

    free(lines);
    munmap(file_data, file_size);
    close(fd);

    return 0;
}