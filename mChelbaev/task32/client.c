#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int mysocket=socket(AF_UNIX, SOCK_STREAM, 0);
    if (mysocket==-1)
    {
        perror("error in create socket");
    }
    struct sockaddr_un serveraddr;
    serveraddr.sun_family=AF_UNIX;
    strncpy(serveraddr.sun_path, "socket", sizeof(serveraddr.sun_path)-1);
    printf("readyclient\n");
    if (connect(mysocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr))==-1)
    {
        perror("error in connect");
    }
    char* str=(char*)malloc(sizeof(char)*100);
    printf("Input\n");
    while(fgets(str, 100, stdin)!=0)
    {
        write(mysocket, str, strlen(str));
    }
    free(str);
    close(mysocket);
    exit(0);
}
