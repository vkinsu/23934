#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void check(uid_t *real_uid, uid_t *effective_uid) {
    printf("Real User ID = %d\n", *real_uid);
    printf("Effective User ID = %d\n", *effective_uid);

    FILE *f = fopen("test.txt", "r+");

    if (f == NULL) perror("Error: Could not open file");
    else
    {
        fclose(f);
    }
}

int main() {
    uid_t real_uid = getuid();
    uid_t effective_uid = geteuid();

    check(&real_uid, &effective_uid);

    setuid(real_uid);

    effective_uid = geteuid();

    check(&real_uid, &effective_uid);

    return 0;
}
