#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>



#define FILE_NAME_LEN 100
#define MAX_NUMB_OF_LINES 1000
#define MAX_STR_LEN 200

typedef struct lineInfo {
    size_t length;
    off_t offset;
} lineInfo;

int countStr = 0;
lineInfo* lineTable;
char filename[FILE_NAME_LEN];


void makeIndentationTable() {
    FILE* fin = fopen(filename, "r");
    if (fin == NULL) {
        perror("failed to open file\n");
        return;
    }

    lineTable = (lineInfo*)malloc(MAX_NUMB_OF_LINES * sizeof(lineInfo));
    if (lineTable == NULL) {
        perror("failed to allocate memory\n");
        fclose(fin);
        return;
    }

    char tempStr[MAX_STR_LEN];
    off_t currentOffset = 0;
    while (fgets(tempStr, MAX_STR_LEN, fin) != NULL) {
        lineTable[countStr].length = strlen(tempStr);
        lineTable[countStr].offset = currentOffset;

        currentOffset += strlen(tempStr);
        countStr += 1;
    }
    fclose(fin);

}

void printLinesByNumber() {
    struct stat sb;
    char* mapped_file;

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("failed to open file");
        return;
    }

    if ( fstat(fd, &sb) == -1 ) perror ("fstat error");

    int numbsrting = 0;
    mapped_file = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    while (1) {
        printf("Enter line number (0 to exit):\n");
        scanf("%d", &numbsrting);

        if (numbsrting == 0) {
            break;
        }

        if (numbsrting > countStr || numbsrting < 1) {
            printf("uncorrect numb of string\n");
            continue;
        }

        char tempString[MAX_STR_LEN];
        int idx = 0;
        while(idx<lineTable[numbsrting - 1].length){
            tempString[idx] = mapped_file[lineTable[numbsrting - 1].offset + idx];
            idx++;
        }

        tempString[lineTable[numbsrting - 1].length] = '\0';
        printf("%s\n", tempString);
    }
    close(fd);
}

int main(void) {

    printf("Enter file name, please :\n");
    if (fgets(filename,FILE_NAME_LEN, stdin) == NULL) {
        perror("failed to read file name");
    }
    else {
        size_t len = strlen(filename);
        if (len > 0 && filename[len - 1] == '\n') {
            filename[len - 1] = '\0';
        }
    }

    makeIndentationTable();
    if (lineTable == NULL) {
        return 1;
    }

    printLinesByNumber();

    free(lineTable);
    return 0;
}
