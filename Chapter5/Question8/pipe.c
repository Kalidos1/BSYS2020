#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>

int main(int argc, char *argv[]) {
    int fd[2], nbytes;
    pid_t childpid;
    char string[] = "Hello new Process!\n";
    char readbuffer[80];

    pipe(fd);

    if ((childpid = fork()) == -1) {
        perror("fork");
        exit(1);
    }
    if (childpid == 0)
    {
        close(fd[1]);
        nbytes = read(fd[0], readbuffer, sizeof(string) + 1);
        printf("Received string: %s", readbuffer);
        exit(0);
    } else
    {
        close(fd[0]);
        write(fd[1], string, strlen(string) + 1);
        wait(0);
    }
    return 0;
 }

 /*
  * Question 6a:
  *
  */