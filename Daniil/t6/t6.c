#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <time.h>

#define BUF_SIZE 4096
#define TIMEOUT 5  // 5 секунд на ввод

// Функция для построения таблицы отступов и длин строк
int build_offset_table(int fd, off_t **offsets, size_t *line_count) {
    char buffer[BUF_SIZE];
    off_t current_offset = 0;
    ssize_t bytes_read;
    size_t count = 0;
    *offsets = malloc(sizeof(off_t)); // Временная таблица, которая будет увеличиваться по мере необходимости
    (*offsets)[count++] = 0;  // Начальная строка начинается с отступа 0

    while ((bytes_read = read(fd, buffer, BUF_SIZE)) > 0) {
        for (ssize_t i = 0; i < bytes_read; i++) {
            current_offset++;
            if (buffer[i] == '\n') {
                *offsets = realloc(*offsets, (count + 1) * sizeof(off_t));
                (*offsets)[count++] = current_offset;
            }
        }
    }

    *line_count = count;
    return 0;
}

// Функция для вывода строки по номеру
int print_line(int fd, off_t *offsets, size_t line_count, size_t line_number) {
    if (line_number >= line_count) {
        printf("Строка %zu не существует.\n", line_number);
        return -1;
    }

    off_t start = line_number == 0 ? 0 : offsets[line_number - 1];
    off_t end = offsets[line_number];
    size_t length = end - start;

    char *buffer = malloc(length + 1);
    // Позиционирование на начало строки
    lseek(fd, start, SEEK_SET);
    read(fd, buffer, length);
    buffer[length] = '\0';  // Завершить строку символом конца строки
    printf("%s", buffer);

    free(buffer);
    return 0;
}

// Функция для вывода всего содержимого файла
void print_file_contents(int fd) {
    char buffer[BUF_SIZE];
    ssize_t bytes_read;

    lseek(fd, 0, SEEK_SET);  // Позиционируемся на начало файла
    while ((bytes_read = read(fd, buffer, BUF_SIZE)) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }
}

// Функция для очистки ввода при ошибке
void clear_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    off_t *offsets = NULL;
    size_t line_count = 0;

    // Строим таблицу отступов
    build_offset_table(fd, &offsets, &line_count);

    // Печатаем таблицу для отладки
    printf("Таблица отступов:\n");
    for (size_t i = 0; i < line_count; i++) {
        printf("Строка %zu начинается с отступа %ld\n", i, (long)offsets[i]);
    }

    fd_set set;
    struct timeval timeout;
    size_t line_number;
    int result;
    
    while (1) {
        FD_ZERO(&set);          // Очищаем набор файловых дескрипторов
        FD_SET(STDIN_FILENO, &set);  // Добавляем стандартный ввод в набор

        timeout.tv_sec = TIMEOUT;    // Устанавливаем таймаут 5 секунд
        timeout.tv_usec = 0;

        printf("Введите номер строки (0 для выхода, %d секунд на ввод): ", TIMEOUT);
        fflush(stdout);  // Очищаем буфер, чтобы текст сразу отобразился

        result = select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout);
        if (result == 0) {
            // Время истекло, выводим всё содержимое файла
            printf("\nВремя вышло! Содержимое файла:\n");
            print_file_contents(fd);
            break;
        } else {
            // Ввод пользователя
            if (scanf("%zu", &line_number) != 1) {
                printf("Некорректный ввод! Попробуйте еще раз.\n");
                clear_input();  // Очищаем буфер ввода
                continue;       // Снова предлагаем ввести строку
            }

            if (line_number == 0) {
                break;
            }

            print_line(fd, offsets, line_count, line_number);
        }
    }

    free(offsets);
    close(fd);
    return 0;
}