#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/poll.h>

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
    listen(mysocket, 100);
    printf("readyserver\n");
    struct pollfd fds1[1];
    struct pollfd fds2[100];
    int count, count2;
    int newsocket[100];
    int k=0;
    char* str=(char*)malloc(sizeof(char)*100);
    fds1[0].fd=mysocket;
    fds1[0].events=POLLIN;
    while(1)
    {
        count=poll(fds1, 1, 1000);
        if (fds1[0].revents & POLLIN)
        {
            fds1[0].revents=0;
            printf("poll succes\n");
            newsocket[k]=accept(mysocket, NULL, NULL);
            if (newsocket[k]==-1)
            {
                perror("error in accept");
            }
            fds2[k].fd=newsocket[k];
            fds2[k].events=POLLRDNORM;
            k++;
        }
        count2=poll(fds2, 100, 1000);
        for (int j=0; j<k; j++)
        {
            if (fds2[j].revents & POLLRDNORM)
            {
                printf("Write\n");
                ssize_t s=read(newsocket[j], str, 100);
                if (s==0)
                {
                    close(newsocket[j]);
                    break;
                }
                for (int i=0; i<strlen(str); i++)
                { 
                    str[i]=(char)toupper((int)str[i]);
                }
                printf("%s", str);
            }
        }
    }
    free(str);
    close(mysocket);
    unlink("socket");
    exit(0);
}
