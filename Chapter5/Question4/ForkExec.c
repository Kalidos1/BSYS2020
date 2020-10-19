#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>

int main(int argc, char *argv[]) {
    int rc = fork();
    if (rc < 0) {
        // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc==0) {
        // child (new process)
        char *myargs[2];
        myargs[0] = strdup("/bin/ls");
        myargs[1] = NULL;
        execvp(myargs[0], myargs);
        printf("hello, I am child (pid:%d)\n", (int) getpid());
    }  else {
        // parent goes down this path (main)
        int wc = wait(NULL);
        printf("hello, I am parent of %d (wc:%d) (pid:%d)\n",
               rc, wc, (int) getpid());
    }
    return 0;
 }

 /*
  * Question 4a:
  * Every Exec() Method works
  * execve() and execvP() do need 3 Parameters => 1. argv, 2. searchpath, but these work also
  * The other ones all Take 2 Parameters and everything does what is is supposed to
  *
  */