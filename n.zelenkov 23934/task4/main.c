#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char *data;
    struct Node *prev;
};

void add_node(struct Node **head, const char *str) {
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
    if (new_node == NULL) {
        perror("Ошибка выделения памяти для узла");
        exit(EXIT_FAILURE);
    }

    new_node->data = (char *)malloc(strlen(str) + 1); // Выделение памяти для строки
    if (new_node->data == NULL) {
        perror("Ошибка выделения памяти для строки");
        exit(EXIT_FAILURE);
    }
    
    strcpy(new_node->data, str);
    new_node->prev = *head; // Добавление узла в начало списка
    *head = new_node;
}

void print_list_reverse(struct Node *node) {
    if (node == NULL) return;
    print_list_reverse(node->prev); // Рекурсивный вызов для следующего узла
    printf("%s", node->data);
}

void free_list(struct Node *head) {
    while (head != NULL) {
        struct Node *prev_node = head->prev;
        free(head->data);
        free(head);
        head = prev_node;
    }
}

int main() {
    struct Node *head = NULL;
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    printf("Введите строки (введите '.' для завершения):\n");

    while (1) {
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            perror("Ошибка ввода");
            exit(EXIT_FAILURE);
        }
        if (buffer[0] == '.') break;
        add_node(&head, buffer);
    }

    printf("\nВы ввели:\n");
    print_list_reverse(head);
    free_list(head);

    return 0;
}

