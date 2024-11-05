#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>

#define MAX_LINES 500

int main()
{
    long line_positions[MAX_LINES] = {0};
    int line_lengths[MAX_LINES] = {0};
    int file_descriptor;
    int line_count = 0;
    char current_char;
    char buffer[257];

    if ((file_descriptor = open("test.txt", O_RDONLY)) == -1)
    {
        perror("Failed to open file");
        exit(1);
    }

    line_positions[0] = 0;

    while (read(file_descriptor, &current_char, 1) > 0)
    {
        if (current_char == '\n')
        {
            line_lengths[line_count] = line_count == 0 ? 1 : (line_positions[line_count] - line_positions[line_count - 1]);
            line_positions[++line_count] = lseek(file_descriptor, 0L, SEEK_CUR);
        }
    }

    if (line_count > 0 && (current_char != '\n'))
    {
        line_lengths[line_count] = lseek(file_descriptor, 0L, SEEK_CUR) - line_positions[line_count - 1];
        line_count++;
    }

    while (1)
    {
        printf("Enter line number (0 to exit): ");
        int result = scanf("%d", &line_count);

        if (result != 1)
        {
            fprintf(stderr, "Invalid input. Please enter a valid number.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        if (line_count == 0)
        {
            close(file_descriptor);
            exit(0);
        }

        if (line_count < 1 || line_count > line_count)
        {
            printf("Invalid line number\n");
            continue;
        }

        lseek(file_descriptor, line_positions[line_count - 1], SEEK_SET);
        memset(buffer, 0, sizeof(buffer));

        if (read(file_descriptor, buffer, line_lengths[line_count - 1]) > 0)
        {
            buffer[line_lengths[line_count - 1]] = '\0';
            printf("%s\n", buffer);
        }
        else
        {
            printf("Error reading line\n");
        }
    }

    close(file_descriptor);
    return 0;
}
