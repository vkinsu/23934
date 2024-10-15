#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char** argv){
    printf("EUID = %d\n", geteuid());
    printf ("RUID = %d\n", getuid());

    if (argc == 2){
        FILE* file = fopen(argv[1], "r");
        if (NULL == file){ perror("ne mogu otkrit"); return 1; }
        printf("mogu otkrit %s\n", argv[1]);

        fclose(file);

        setuid(getuid());

        file = fopen(argv[1], "r");
                if (NULL == file){ perror("ne mogu otkrit"); return 1; }
                printf("mogu otkrit %s\n", argv[1]);
                fclose(file);
    }
    return 0;
}
