#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Получаем значение переменной окружения TZ
    setenv("TZ", "America/Los_Angeles", 1);
    const char *tz = getenv("TZ");

    if (tz) {
        printf("Текущая временная зона (TZ): %s\n", tz);
    } else {
        printf("Переменная окружения TZ не установлена.\n");
    }

    return 0;
}