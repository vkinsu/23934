#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main() {

    char line1[50];

    FILE *f;
    f = fopen("file.txt", "r");
    z5

    int table[10000];
    int i=0;
    while (fgets(line1, 50, f) != NULL) {
        table[i] = strcspn(line1, "\n");
        i++;
    }
    fclose(f);


    while (1)
    {
        f = fopen("file.txt", "r");
        printf("%s", "\nEnter number string ");
        int n;
        scanf("%d", &n);
        if (n == 0) return 0;
        n--;
        int count_deap=0;

        for (int j=0;j<n;j++)
        {
            count_deap+=(table[j]+2);
        }
        if (n!=0)
            fseek(f, count_deap, SEEK_SET);
        char* line = malloc((table[n]+1)*sizeof (char));
        fgets(line, (table[n]+1), f);
        printf("%s",line);
        free(line);

        fclose(f);
    }

}
