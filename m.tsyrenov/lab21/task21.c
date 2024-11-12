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
}
void mysignal2(int var)
{
    printf("\n%d\n", k);
    exit(0);
}
int main()
{
    while(1)
    {
	signal(SIGINT, mysignal);
	signal(SIGQUIT, mysignal2);
    }
    exit(0);
}
