#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main() {
    time_t now;
    struct tm *sp;

    (void) time(&now);
    setenv("TZ", "PST8PDT", 1);
    printf("%s", ctime(&now));

    return 0;
}

