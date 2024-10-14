#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 256

typedef struct Node {
    char *string;
    struct Node *next;
} Node;

Node *head = NULL, *tail = NULL;

void init() {
    head = NULL;
    tail = NULL;
}

void push(const char *string) {
    Node *newNode = malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    newNode->string = strdup(string); // копируем строку с выделением памяти
    newNode->next = NULL;

    if (tail) {
        tail->next = newNode;
    } else {
        head = newNode; // если список пустой, то head указывает на новый узел
    }
    tail = newNode; // tail всегда указывает на последний узел
}

void printList() {
    Node *ptr = head;
    while (ptr) {
        if (ptr->string) {
            printf("%s\n", ptr->string);
        }
        ptr = ptr->next;
    }
}

void freeList() {
    Node *ptr = head;
    while (ptr) {
        Node *temp = ptr;
        ptr = ptr->next;
        free(temp->string);
        free(temp);
    }
}

int main() {
    char inputBuf[BUFFER_SIZE];

    init();

    while (fgets(inputBuf, BUFFER_SIZE, stdin)) {
        if (inputBuf[0] == '.') {
            printList();
            freeList(); // освобождаем память
            return 0;
        }

        char *lineEnd = strchr(inputBuf, '\n');
        if (lineEnd) {
            *lineEnd = '\0'; // удаляем символ новой строки
        }

        push(inputBuf); // добавляем строку в список
    }

    freeList(); // освобождаем память после выхода из цикла
    return 0;
}

