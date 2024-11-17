#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int fd, len=40, pos=0, numberline=0, flag, k;
    char s;
    char** lines=(char**)malloc(sizeof(char*)*512);
    for (int i=0; i<512; i++)
    {
        lines[i]=(char*)malloc(sizeof(char)*41);
    }
    struct termios tty, savetty;
    fd = open("/dev/tty", O_RDONLY);
    tcgetattr(fd, &tty);
    savetty=tty;
    tty.c_lflag &=~(ISIG | ICANON | ECHO);
    tty.c_cc[VMIN] = 1;
    tcsetattr(fd, TCSAFLUSH, &tty);
    while(1)
    {
        scanf("%c", &s);
        printf("%c", s);
        if ((int)s==127)
        {
            printf("\b \b");
            lines[numberline][pos]='\0';
            pos--;
            continue;
        }
        if ((int)s==21)
        {
            printf("\r");
            for (int i=39; i>=0; i--)
            {
                lines[numberline][i]='\0';
                printf(" ");
            }
            printf("\r");
            pos=0;
            continue;
        }
        if ((int)s==23)
        {
            while(pos>0 && lines[numberline][pos-1]==' ')
            {
                lines[numberline][pos]='\0';
                printf("\b");
                pos--;
            }
            while(pos>0 && lines[numberline][pos-1]!=' ')
            {
                lines[numberline][pos]='\0';
                printf("\b \b");
                pos--;
            }
            continue;
        }
        if ((int)s==4 && pos==0)
        {
            break;
        }
        if ((int)s==10)
        {
            numberline++;
            pos=0;
        }
        if ((int)s<32)
        {
            printf("\a");
            continue;
        }
        lines[numberline][pos]=s;
        pos++;
        if (pos>40)
        {
            if ((int)s!=32)
            {
                k=0;
                for (int i=pos-1; i>=0; i--)
                {
                    if (lines[numberline][i]!=' ')
                    {
                        k++;
                    }
                    else
                    {
                        break;
                    }
                }
                if (k<=40)
                {
                    for (int i=0; i<k; i++)
                    {
                        printf("\b \b");
                    }
                    printf("\n");
                    for (int i=k; i>0; i--)
                    {
                        printf("%c", lines[numberline][pos-i]);
                        lines[numberline][pos-i]='\0';
                    }
                    numberline++;
                    pos=k;
                }
                else
                {
                    printf("\b \b");
                    printf("\n");
                    printf("%c", s);
                    numberline++;
                    pos=1;
                }
                
            }
            else
            {
                printf("\b\n ");
                numberline++;
                pos=0;
            }
        }
    }
    tcsetattr(fd, TCSAFLUSH, &savetty);
    free(lines);
    exit(0);
}
