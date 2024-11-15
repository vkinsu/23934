#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>

#define MAX_LENGTH_TEXT 40

const char *red = "\033[31m";
const char *reset = "\033[0m";
const char *green = "\033[32m";
const char *purple = "\033[35m";


int main() {
    int client_sock = socket(AF_UNIX,SOCK_STREAM,0);

    if (client_sock == -1) {
        printf("%sError: failed to create socket %s\n", red, reset);
        exit(1);
    }

    printf("%sThe client ready to connect%s\n", green, reset);
    
    struct sockaddr_un addr;
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "../server/socket" ,sizeof(addr.sun_path));
    
    if (connect(client_sock,(struct sockaddr *)&addr,sizeof(addr))==-1){
        printf("%sError: failed to connect server%s\n", red, reset);
        exit(1);
    }
    
    char tempText[MAX_LENGTH_TEXT];
    
    while (1) {
        if (fgets(tempText, sizeof(tempText), stdin) == NULL) break; // Проверка на EOF
        if (strlen(tempText) > MAX_LENGTH_TEXT - 1) {
            printf("%sERROR: The entered text is too long! Maximum %d characters.%sn", red, MAX_LENGTH_TEXT - 1, reset);
            continue;
            }
            size_t len = strlen(tempText);
            if (len > 0 && tempText[len - 1] == '\n') {
                tempText[len - 1] = '\0';
            }
            write(client_sock, tempText, strlen(tempText) + 1); 
    }
    close(client_sock);   
}
