#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char str[1024];
    char** list=(char**) malloc(sizeof(char*)*2);
    int len=0;
    int count=0;
    while(1)
    {
        fgets(str, 1024, stdin);
        if (str[0]=='.')
        {
            break;
        }
        len=strlen(str);
        list[count]=(char*)malloc(sizeof(char)*(len+1));
        for (int i=0; i<len; i++)
        {
            list[count][i]=str[i];
        }
        list[count][len]='\0';
        count+=1;
        if (count%2==0)
        {
            list=(char**) realloc(list, sizeof(char*)*count*2);
        }
    }
    for (int i=0; i<count; i++)
    {
        printf("%s", list[i]);
    }
    for (int i=0; i<count; i++)
    {
        free(list[i]);
    }
    free(list);
    exit(0);
}
