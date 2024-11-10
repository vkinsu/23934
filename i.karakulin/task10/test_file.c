#include <stdlib.h>

int main()
{
    unsigned int i = 0;

    while (i < (1U << 31))
        i++;

    return 0;
}