#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define ERROR_FILE_MSG "Error: Can't create new file"
#define ERROR_WRITE "Error: Can't write data in file"
#define ERROR_CLOSE "Error: Can't close file"
#define ERROR_LSEEK "Error: Can't seek in file"
#define MAX_SIZE 4 * 1024 * 1024

int main(void) {
    int fd = open("A", O_TRUNC | O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
    if (fd == -1) {
	fprintf(stderr, ERROR_FILE_MSG);
        exit(EXIT_FAILURE);
    }
    int i = 0;
    if (write(fd, "1", 1) != 1) {
	fprintf(stderr, ERROR_WRITE);
        exit(EXIT_FAILURE);
    }
    i++;
    for (i; i < 10000; i++) {
	write(fd, "\0", SEEK_CUR);
    }

    if (write(fd, "1", 1) != 1) {
	fprintf(stderr, ERROR_WRITE);
        exit(EXIT_FAILURE);
    }

    for (i++; i < 4 * 1024 * 1024; i++) {
	write(fd, "\0", SEEK_CUR);
    }
    
    if (write(fd, "1", 1) != 1) {
	fprintf(stderr, ERROR_WRITE);
        exit(EXIT_FAILURE);
    }
    if (close(fd) == -1) {
	fprintf(stderr, ERROR_CLOSE);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
