#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(){

    setenv("TZ", "America/Los_Angeles", 1);

    time_t current_time;
    time(&current_time);

    struct tm *local_time = localtime(&current_time);
    printf("Текущая дата и время: %02d-%02d-%04d %02d:%02d:%02d\n",
           local_time->tm_mday,
           local_time->tm_mon + 1,
           local_time->tm_year + 1900, // Год начинается с 1900
           local_time->tm_hour,
           local_time->tm_min,
           local_time->tm_sec);

    return 0;
}