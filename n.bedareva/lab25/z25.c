#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>


int main() {
    int pipefd[2];
    pid_t forkk;
    char buffer[100];

    pipe(pipefd);
    forkk = fork();

    if (forkk== 0) {
        close(pipefd[1]);

        size_t n;
        while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            for (size_t i = 0; i < n; i++) {
                buffer[i] = toupper((unsigned char)buffer[i]);
            }
            write(STDOUT_FILENO, buffer, n);
        }

        close(pipefd[0]); 
        return 0;
    } else { 
        close(pipefd[0]); 

        const char *text = "Hello, World\n";
        write(pipefd[1], text, strlen(text));

        close(pipefd[1]); 
        return 0;
    }
}
