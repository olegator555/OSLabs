#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char** argv) {
    int fd_pipe1[2];
    int fd_pipe2[2];
    int fd_pipe3[2];
    int fd_pipe4[2];
    pipe(fd_pipe1);
    pipe(fd_pipe2);
    pipe(fd_pipe3);
    pipe(fd_pipe4);

    if (argc < 2) {
        write(STDIN_FILENO, "File name not specified\n", 38);
        exit(0);
    }

    if (fork() == 0) {
        close(STDOUT_FILENO);
        dup(fd_pipe1[STDOUT_FILENO]);
        close(fd_pipe1[STDOUT_FILENO]);
        close(fd_pipe1[STDIN_FILENO]);
        execl("/bin/cat", "cat", argv[1], (char *) 0);
        exit(1);
    }

    if (fork() == 0) {
        close(STDIN_FILENO);
        dup(fd_pipe1[STDIN_FILENO]);
        close(fd_pipe1[STDIN_FILENO]);
        close(fd_pipe1[STDOUT_FILENO]);
        close(STDOUT_FILENO);
        dup(fd_pipe2[STDOUT_FILENO]);
        close(fd_pipe2[STDOUT_FILENO]);
        execl("/usr/bin/tr", "tr", "-cs", "A-Za-z", "\n", (char *) 0);
        exit(2);
    }

    if (fork() == 0) {
        close(STDIN_FILENO);
        dup(fd_pipe2[STDIN_FILENO]);
        close(fd_pipe2[STDIN_FILENO]);
        close(fd_pipe2[STDOUT_FILENO]);
        close(fd_pipe1[STDIN_FILENO]);
        close(fd_pipe1[STDOUT_FILENO]);
        close(STDOUT_FILENO);
        dup(fd_pipe3[STDOUT_FILENO]);
        close(fd_pipe3[STDOUT_FILENO]);
        close(fd_pipe3[STDIN_FILENO]);
        execl("/usr/bin/sort", "sort", (char *) 0);
        exit(3);
    }

    if (fork() == 0) {
        close(STDIN_FILENO);
        dup(fd_pipe3[STDIN_FILENO]);
        close(fd_pipe3[STDIN_FILENO]);
        close(fd_pipe3[STDOUT_FILENO]);
        close(fd_pipe1[STDIN_FILENO]);
        close(fd_pipe1[STDOUT_FILENO]);
        close(fd_pipe2[STDIN_FILENO]);
        close(fd_pipe2[STDOUT_FILENO]);
        close(STDOUT_FILENO);
        dup(fd_pipe4[STDOUT_FILENO]);
        close(fd_pipe4[STDOUT_FILENO]);
        execl("/usr/bin/uniq", "uniq", (char *) 0);
        exit(4);
    }

    if (fork() == 0) {
        close(STDIN_FILENO);
        dup(fd_pipe4[STDIN_FILENO]);
        close(fd_pipe4[STDIN_FILENO]);
        close(fd_pipe4[STDOUT_FILENO]);
        close(fd_pipe1[STDIN_FILENO]);
        close(fd_pipe1[STDOUT_FILENO]);
        close(fd_pipe2[STDIN_FILENO]);
        close(fd_pipe2[STDOUT_FILENO]);
        close(fd_pipe3[STDIN_FILENO]);
        close(fd_pipe3[STDOUT_FILENO]);
        char temp[4096];
        sprintf(temp, "%s%s%s", "^", argv[2], "[A-Za-z]*");
        execl("/bin/grep", "grep", temp, (char *) 0);
        exit(5);
    }

    close(fd_pipe1[STDIN_FILENO]);
    close(fd_pipe1[STDOUT_FILENO]);
    close(fd_pipe2[STDIN_FILENO]);
    close(fd_pipe2[STDOUT_FILENO]);
    close(fd_pipe3[STDIN_FILENO]);
    close(fd_pipe3[STDOUT_FILENO]);
    close(fd_pipe4[STDIN_FILENO]);
    close(fd_pipe4[STDOUT_FILENO]);
    while (wait(0) != (-1));
    exit(0);
}
