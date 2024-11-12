#include <stdio.h>
#define SIZE 2056

int main()
{
    char buffer[SIZE];
    FILE *fp = fopen("text.txt", "r");

    if (fp)
    {
        while ((fgets(buffer, SIZE, fp)) != NULL)
        {
            printf("%s", buffer);
        }
        fclose(fp);
    }
}
