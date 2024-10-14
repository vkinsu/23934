#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/select.h>

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

void read_line(const char *mapped_file, Line line) {
    fwrite(mapped_file + line.offset, 1, line.length, stdout);
    printf("\n");
}

void print_all_file(const char *mapped_file, const Array *table) {
    int i;
    for (i = 0; i < table->cnt; i++) {
        read_line(mapped_file, table->array[i]);
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

    // Открываем файл
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        handle_error("Failed to open file");
    }

    // Получаем размер файла
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        handle_error("Failed to get file size");
    }
    size_t file_size = sb.st_size;

    // Отображаем файл в память
    char *mapped_file = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped_file == MAP_FAILED) {
        handle_error("Failed to map file");
    }

    // Закрываем файловый дескриптор, так как он больше не нужен
    close(fd);

    // Чтение строк из отображённого файла
    off_t lineOffset = 0;
    off_t lineLength = 0;
    off_t i;
    for (i = 0; i < file_size; i++) {
        if (mapped_file[i] == '\n') {
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
        fflush(stdout);

        if (wait_for_input(TIMEOUT_SECONDS) == 0) {
            printf("\nTimeout! Printing the entire file:\n");
            print_all_file(mapped_file, &table);
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

        read_line(mapped_file, table.array[num - 1]);
    }

    // Освобождаем ресурсы
    munmap(mapped_file, file_size);
    freeArray(&table);

    return 0;
}

