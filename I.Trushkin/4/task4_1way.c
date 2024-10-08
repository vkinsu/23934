#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STR_LENGTH_MAX 1000

typedef struct vector {
    int cap;
    int size;
    char** strings;
}vector;

char* strcpyWithAllocation (char* src) {
    char* newStr = (char*)malloc(sizeof(char)* (strlen(src) + 1));
    strcpy(newStr,src);
    return newStr;
}

vector initVec () {
    vector vec;
    vec.cap = 2;
    vec.size = 0;
    vec.strings = (char**)malloc(sizeof(char*) * vec.cap);
    return vec;
}

void vecReallocate(vector *vec) {
    vec->cap *= 2;
    vec->strings = (char**)realloc(vec->strings,vec->cap*sizeof(char*));
}

void vecAddElem (vector* vec ,char* str) {
    if (vec->cap == vec->size) {
        vecReallocate(vec);
    }
    vec->strings[(vec->size)++] = strcpyWithAllocation(str);
}

int main() {

    vector listStrings = initVec();

    char tempString[STR_LENGTH_MAX] = "";

    while(fgets(tempString,STR_LENGTH_MAX,stdin)!= NULL) {
        if (tempString[0] == '.') break;

        vecAddElem(&listStrings,tempString);
    }

    for (int i = 0; i<listStrings.size ; i++) {
        printf("%s",listStrings.strings[i]);
    }

    return 0;
}