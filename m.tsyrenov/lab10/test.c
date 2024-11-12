#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    printf("Success\n");
    for (int i=0; i<argc; i++)
    {
        printf("%s\n", argv[i]);
    }
    exit(0);
}
