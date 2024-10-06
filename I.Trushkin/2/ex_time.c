#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    setenv("TZ", "America/Los_Angeles", 1);
    time_t now;
    now = time(&now);
    printf("%s", ctime(&now));
    return 0;
}
