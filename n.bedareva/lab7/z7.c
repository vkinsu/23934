#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


int main() {

    char line1[1000];
  

    FILE *f;
    f = fopen("file.txt", "r");

    if (!f) return 0;

    char *table[1000];
    int i=0;
    while (fgets(line1, 1000, f) != NULL) {
        table[i] = malloc(sizeof (char) * strlen(line1));
        strcpy(table[i], line1);
        i++;
    }
    fclose(f);


    while (1)
    {

         fd_set readfds;
         struct timeval timeout;
    
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);

        int result = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);

        if(result == 0)
        {
                for(int j=0;j<i;j++)
                {
                        printf("%s",table[j]);
                }
                return 0;
        }
        f = fopen("file.txt", "r");

        printf("Enter number string: \n");
        int n;
        scanf("%d", &n);
        if (n == 0) return 0;
        n--;
        printf("%s",table[n]);

        fclose(f);
    }

}

