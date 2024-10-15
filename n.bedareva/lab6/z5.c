#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

int main() {

    char line1[50];

    FILE *f;
    f = fopen("file.txt", "r");

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
        
         fd_set readfds;
    struct timeval timeout;
    

    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    int result = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);
    
    	if(result == 0)
    	{
    		char line2[200];
    		while (fgets(line2, 200, f) != NULL) {
        		printf("%s",line2);
        		i++;
    		}
    		return 0;
    	}
    
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
