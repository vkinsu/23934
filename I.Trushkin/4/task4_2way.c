#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STR_LENGTH_MAX 1000

typedef struct listNode{
    char* string;
    struct listNode* next ;
}listNode;

listNode* head = NULL;
listNode* last = NULL;

char* strcpyWithAllocation (char* src) {
    char* newStr = (char*)malloc(sizeof(char)* (strlen(src) + 1));
    strcpy(newStr,src);
    return newStr;
}

listNode* newElem (char* str) {
    listNode *new = (listNode*)malloc(sizeof(listNode));
    new->string = strcpyWithAllocation(str);
    new->next = NULL;
    return new;
}

void addElem (char* str) {
    if (head == NULL) {
        head = newElem(str);
        last = head;
    }
    else {
        last->next = newElem(str);
        last = last->next;
    }

}

void printList() {
    listNode* temp = head;

    while(temp!=NULL) {
        printf("%s",temp->string);
        listNode* tempTemp = temp->next;
        temp = temp->next;
    }

}

void cleanList() {
    listNode* temp = head;

    while(temp!=NULL) {
        free(temp->string);
        temp = temp->next;
    }
}

int main() {

    char tempString[STR_LENGTH_MAX] = "";

    while(fgets(tempString,STR_LENGTH_MAX,stdin)!= NULL) {
        if (tempString[0] == '.') break;
        addElem(tempString);
    }

    printList();
    cleanList();


    return 0;
}