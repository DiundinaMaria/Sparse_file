#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef BUF_SIZE 
#define BUF_SIZE 4096
#endif
#define ERROR_ARG_MSG "Error: Enter output filename"
#define ERROR_ARG_TOO_MUCH "Error: Too much args"
#define ERROR_FILE_MSG "Error: Can't create new file"
#define ERROR_FILE_OPEN "Error: Not found in file"
#define ERROR_WRITE "Error: Can't write data in file"
#define ERROR_CLOSE "Error: Can't close file"
#define ERROR_LSEEK "Error: Can't seek in file"

int main(int argc, char** argv) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, argc < 2 ? ERROR_ARG_MSG : ERROR_ARG_TOO_MUCH);
        exit(EXIT_FAILURE);
    }

    int fd_in;
    int fd_out;
    if (argc == 3){
        fd_in = open(argv[1], O_RDONLY);
        fd_out = open(argv[2], O_TRUNC | O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    }
    else /* if (argc == 2)*/ {
         fd_out = open(argv[1], O_TRUNC | O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
         fd_in = STDIN_FILENO; // 0
    }
    
    if (fd_in == -1 || fd_out == -1) {
        fprintf(stderr, "%s\n%s\n",
                        (fd_in == -1 ? ERROR_FILE_OPEN : " "),
                        (fd_out == -1 ? ERROR_FILE_MSG : " ")
                );
        exit(EXIT_FAILURE);
    }
    

    char read_buf[BUF_SIZE]; 
    int cur_readed;
    int err_syscall = 0;
    while (err_syscall == 0 && (cur_readed = read(fd_in, read_buf, BUF_SIZE)) != 0) {
	    char *p = read_buf;
	    while (p < read_buf + cur_readed && p[0] == 0) { 
			p++;
	    }
	    if (p == read_buf + cur_readed) { 
		    if (lseek(fd_out, cur_readed, SEEK_CUR) == -1) {
			    fprintf(stderr, ERROR_LSEEK);
			    err_syscall = 1;
		    }
	    } 
	    else if (write(fd_out, read_buf, cur_readed) == -1) {
			fprintf(stderr, ERROR_WRITE);
			err_syscall = 1;
	    }
    }
    
    if (fd_in != STDIN_FILENO) {
        if (close(fd_in) == -1) {
             fprintf(stderr, "%s: %s\n", ERROR_CLOSE, "input");
	     err_syscall = 1;
        }
    }
    if (close(fd_out) == -1) {
        fprintf(stderr, "%s: %s\n", ERROR_CLOSE, "output");
        err_syscall = 1;
    }
    exit(err_syscall == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

