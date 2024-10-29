#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define MAX_LINES 100
#define MAX_LINE_LENGTH 256

typedef struct {
    off_t offset;  // Line offset in file
    size_t length; // Line length
} LineInfo;

void build_line_info(const char *filename, LineInfo *lines, size_t *line_count) 
{
    int fd = open(filename, O_RDONLY);
    if (fd < 0) 
    {
        perror("The file not could be opened");
        exit(EXIT_FAILURE);
    }
    off_t current_offset = 0;
    ssize_t bytes_read;
    char buffer[MAX_LINE_LENGTH];
    size_t line_index = 0;
    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';
        char *line_start = buffer;
        char *newline_pos;
        while ((newline_pos = strchr(line_start, '\n')) != NULL) 
        {
            *newline_pos = '\0'; // Replace '\n' at '\0'
            if (line_index < MAX_LINES) 
            {
                lines[line_index].offset = current_offset;
                lines[line_index].length = strlen(line_start);
                line_index++;
            }
            current_offset += (newline_pos - line_start + 1); //  Increase the current offset           
            line_start = newline_pos + 1; // Let's move on to the next line
        }

        // If there are characters left after the last line feed
        if (*line_start != '\0' && line_index < MAX_LINES) 
        {
            lines[line_index].offset = current_offset;
            lines[line_index].length = strlen(line_start);
            line_index++;
        }
    }
    *line_count = line_index; // Set the total number of lines
    close(fd);
}

void print_line(const char *filename, const LineInfo *lines, size_t line_count, size_t line_number) {
    if (line_number >= line_count) 
    {
        printf("No line with number %zu\n", line_number);
        return;
    }

    int fd = open(filename, O_RDONLY);
    if (fd < 0) 
    {
        perror("The file not could be opened");
        exit(EXIT_FAILURE);
    }

    lseek(fd, lines[line_number].offset, SEEK_SET); // Positioning ourselves at the beginning of the line
    char *line = malloc(lines[line_number].length + 1);
    if (!line) 
    {
        perror("Positioning ourselves at the beginning of the line");
        close(fd);
        exit(EXIT_FAILURE);
    }
    read(fd, line, lines[line_number].length);
    line[lines[line_number].length] = '\0';
    printf("Line %zu: %s\n", line_number, line);
    free(line);
    close(fd);
}
char* file;
LineInfo lines[MAX_LINES];
size_t line_count;
void myclock(int var)
{
    printf("\n");
    for (size_t i=0; i<line_count; i++)
    {
        print_line(file, lines, line_count, i);
    }
    exit(0);
}
int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        fprintf(stderr, "Using: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    file=argv[1];
    build_line_info(argv[1], lines, &line_count);
    printf("Table of lines:\n");
    for (size_t i = 0; i < line_count; i++) 
    {
        printf("Line %zu: Offset %lld, Length %zu\n", i, (long long)lines[i].offset, lines[i].length);
    }
    char s[10];
    size_t line_number = 0;
    int flag;
    int j;
    signal(SIGALRM, myclock);
    while (1) 
    {
        printf("Input number of line (0 for exit): ");
        alarm(5);
        gets(s);
        alarm(0);
	flag=0;
	j=0;
	while (s[j]!='\0' && j<10)
	{
	    if ((int)s[j]<48 || (int)s[j]>57)
	    {
		flag=1;
	    }
	    j+=1;
	}
        line_number=strtol(s, NULL, 10);
	if (flag==1)
	{
	    perror("Wrong Input");
	}
        if (s[0]=='0' && s[1]=='\0') 
        {
            break;
        }
	if (flag==0)
	{
            print_line(argv[1], lines, line_count, line_number - 1);
	} 
    }
    return 0;
}
