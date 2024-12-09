#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

int k=0;
void mysignal(int var)
{
    k+=1;
    printf("\a");
    signal(SIGINT, mysignal);
}
void mysignal2(int var)
{
    printf("\n%d\n", k);
    exit(0);
}
int main()
{
    signal(SIGINT, mysignal);
    signal(SIGQUIT, mysignal2);
    while(1)
    {
	pause();
    }
    exit(0);
}
