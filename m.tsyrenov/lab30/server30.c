#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int main()
{
    int mysocket=socket(AF_UNIX, SOCK_STREAM, 0);
    if (mysocket==-1)
    {
        perror("error in create socket");
    }
    struct sockaddr_un addr;
    addr.sun_family=AF_UNIX;
    strncpy(addr.sun_path, "socket", sizeof(addr.sun_path)-1);
    if (bind(mysocket, (struct sockaddr*)&addr ,sizeof(addr))==-1)
    {
        perror("error in bind");
    }
    listen(mysocket, 1);
    printf("readyserver\n");
    int newsocket=accept(mysocket, NULL, NULL);
    if (newsocket==-1)
    {
        perror("error in accept");
    }
    char* str=(char*)malloc(sizeof(char)*100);
    printf("Write\n");
    while(1)
    {
        ssize_t s=read(newsocket, str, 100);
        if (s==0)
        {
            break;
        }
        for (int i=0; i<strlen(str); i++)
        { 
            str[i]=(char)toupper((int)str[i]);
        }
        printf("%s", str);
    }
    free(str);
    close(newsocket);
    close(mysocket);
    unlink("socket");
    exit(0);
}
