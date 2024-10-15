#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/select.h>
#include <time.h>

#define INITIAL_CAPACITY 1
#define TIMEOUT_SECONDS 5

typedef struct {
    off_t offset;
    off_t length;
} Line;

typedef struct {
    Line *array;
    int cnt;
    int cap;
} Array;

void handle_error(const char *message) {
    perror(message);
    exit(EXIT_FAILURE);
}

void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) handle_error("Failed to allocate memory");
    return ptr;
}

void *safe_realloc(void *ptr, size_t size) {
    ptr = realloc(ptr, size);
    if (!ptr) handle_error("Failed to reallocate memory");
    return ptr;
}

void initArray(Array *a) {
    a->cap = INITIAL_CAPACITY;
    a->cnt = 0;
    a->array = safe_malloc(a->cap * sizeof(Line));
}

void insertArray(Array *a, Line element) {
    if (a->cnt == a->cap) {
        a->cap *= 2;
        a->array = safe_realloc(a->array, a->cap * sizeof(Line));
    }
    a->array[a->cnt++] = element;
}

void freeArray(Array *a) {
    free(a->array);
    a->array = NULL;
    a->cnt = a->cap = 0;
}

void read_line(int fd, Line line) {
    char *buf = safe_malloc(line.length + 1);
    lseek(fd, line.offset, SEEK_SET);
    if (read(fd, buf, line.length) != line.length) {
        free(buf);
        handle_error("Failed to read the line");
    }
    buf[line.length] = '\0';  // добавляем нулевой символ в конец строки
    printf("%s\n", buf);
    free(buf);
}

void print_all_file(int fd, const Array *table) {
    int i;
    for (i = 0; i < table->cnt; i++) {
        read_line(fd, table->array[i]);
    }
}

int wait_for_input(int seconds) {
    fd_set set;
    struct timeval timeout;

    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);

    timeout.tv_sec = seconds;
    timeout.tv_usec = 0;

    return select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *path = argv[1];
    Array table;
    initArray(&table);

    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        handle_error("Failed to open file");
    }

    char c;
    off_t lineOffset = 0;
    off_t lineLength = 0;

    // Читаем файл по символам и сохраняем информацию о строках
    while (read(fd, &c, 1) == 1) {
        if (c == '\n') {
            Line current = {lineOffset, lineLength};
            insertArray(&table, current);
            lineOffset += lineLength + 1;
            lineLength = 0;
        } else {
            lineLength++;
        }
    }

    // Обрабатываем последнюю строку, если она не завершена новой строкой
    if (lineLength > 0) {
        Line current = {lineOffset, lineLength};
        insertArray(&table, current);
    }

    // Основной цикл для ввода номера строки
    while (1) {
        printf("Enter the line number (0 to quit, timeout in %d seconds): ", TIMEOUT_SECONDS);
        fflush(stdout);  // Обязательно сбрасываем буфер для корректного вывода

        if (wait_for_input(TIMEOUT_SECONDS) == 0) {
            printf("\nTimeout! Printing the entire file:\n");
            print_all_file(fd, &table);
            break;
        }

        int num;
        if (scanf("%d", &num) != 1 || num < 0) {
            fprintf(stderr, "Invalid input.\n");
            continue;
        }

        if (num == 0) {
            break;  // Завершаем программу
        }

        if (num > table.cnt) {
            printf("The file contains only %d line(s).\n", table.cnt);
            continue;
        }

        read_line(fd, table.array[num - 1]);
    }

    // Завершающие действия
    close(fd);
    freeArray(&table);

    return 0;
}

