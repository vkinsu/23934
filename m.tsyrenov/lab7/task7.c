#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#define MAX_LINES 100
#define MAX_LINE_LENGTH 256
typedef struct {
    off_t offset;
    size_t length;
} LineInfo;
void build_line_info(const char *filename, LineInfo *lines, size_t *line_count) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) 
    {
        perror("The file not could be opened");
        exit(EXIT_FAILURE);
    }
    struct stat sb;
    if (fstat(fd, &sb) == -1) 
    {
        perror("Could not get information about the file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Displaying the file in memory
    char *file_content = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_content == MAP_FAILED) 
    {
        perror("the file could not be mapped to memory");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd); 
    off_t current_offset = 0;
    size_t line_index = 0;
    for (size_t i = 0; i < sb.st_size; ++i) 
    {
        if (file_content[i] == '\n') 
        {
            if (line_index < MAX_LINES) 
            {
                lines[line_index].offset = current_offset;
                lines[line_index].length = i - current_offset;
                line_index++;
            }
            current_offset = i + 1; 
        }
    }
    if (current_offset < sb.st_size && line_index < MAX_LINES) 
    {
        lines[line_index].offset = current_offset;
        lines[line_index].length = sb.st_size - current_offset;
        line_index++;
    }
    *line_count = line_index; 
    if (munmap(file_content, sb.st_size) == -1) 
    {
        perror("the file display could not be deleted");
        exit(EXIT_FAILURE);
    }
}
char* file_content;
LineInfo lines[MAX_LINES];
size_t line_count=0;
struct stat sb;
void myclock(int var)
{
    putchar('\n');
    for (size_t i=0; i<line_count; i++)
    {
        fwrite(file_content + lines[i].offset, 1, lines[i].length, stdout);
        putchar('\n');
    }
    exit(0);
}
void print_lines(const char *filename, LineInfo *lines, size_t line_count) 
{
    int fd = open(filename, O_RDONLY);
    if (fd < 0) 
    {
        perror("The file not could be opened");
        exit(EXIT_FAILURE);
    }
    if (fstat(fd, &sb) == -1) 
    {
        perror("Could not get information about the file");
        close(fd);
        exit(EXIT_FAILURE);
    }
    file_content = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (file_content == MAP_FAILED) 
    {
        perror("the file could not be mapped to memory");
        close(fd);
        exit(EXIT_FAILURE);
    }
    close(fd);
    printf("Table of lines\n");
    for (size_t i=0; i<line_count; i++)
    {
	printf("Line %zu Offset %zu Length %zu\n", (i+1), lines[i].offset, lines[i].length);
    } 
    size_t line_number;
    char s[10];
    int flag;
    int j;
    signal(SIGALRM, myclock);
    while (1) 
    {
        printf("Input number of line (1 to %zu): ", line_count);
	alarm(5);
        gets(s);
        alarm(0);
        line_number=strtol(s, NULL, 10);
	flag=0;
	j=0;
	while(s[j]!='\0' && j<10)
	{
	    if ((int)s[j]<48 || (int)s[j]>57)
	    {
		flag=1;
	    }
	    j+=1;
	}
	if (flag==1)
	{
	    perror("Wrong input");
        }
        if (s[0]=='0' && s[1]=='\0')
        {
            break;
        }
	if (flag==0)
	{
            off_t offset = lines[line_number - 1].offset;
            size_t length = lines[line_number - 1].length;
            fwrite(file_content + offset, 1, length, stdout);
            putchar('\n');
	}
    }
    if (munmap(file_content, sb.st_size) == -1) 
    {
        perror("the file display could not be deleted");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) 
{
    if (argc!=2) 
    {
        fprintf(stderr, "Using: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    build_line_info(argv[1], lines, &line_count);
    print_lines(argv[1], lines, line_count);
    exit(EXIT_SUCCESS);
}
