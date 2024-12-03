#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <aio.h>
#include <signal.h>
#include <ctype.h>
#include <sys/time.h>

struct aiocb *aiocbp2;
void timer_callback(int sig)
{
    aio_read(aiocbp2);
    printf("%s", aiocbp2->aio_buf);
}
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
    char* str2=(char*)malloc(sizeof(char)*100);
    struct itimerval newTimer;
    struct itimerval oldTimer;
    newTimer.it_value.tv_sec = 0;
    newTimer.it_value.tv_usec = 100*1000;
    newTimer.it_interval.tv_sec = 0;
    newTimer.it_interval.tv_usec = 100 * 1000;
    setitimer(ITIMER_REAL, &newTimer, &oldTimer);
    signal(SIGALRM, timer_callback);
    struct aiocb *aiocbp =calloc(1, sizeof(struct aiocb));
    aiocbp->aio_fildes=mysocket;
    aiocbp->aio_buf=str;
    aiocbp->aio_nbytes=100;
    aiocbp2=calloc(1, sizeof(struct aiocb));
    aiocbp2->aio_fildes=mysocket;
    aiocbp2->aio_buf=str2;
    aiocbp2->aio_nbytes=100;
    printf("Input\n");
    while(fgets(str, 100, stdin))
    {
        write(mysocket, str, 100);
    }
    close(mysocket);
    exit(0);
}
