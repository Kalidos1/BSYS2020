#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    close(STDOUT_FILENO);
    open("./fork2.output", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
    int rc = fork();
    if (rc < 0) {
        // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc==0) {
        // child (new process)
        printf("hello, I am child (pid:%d)\n", (int) getpid());
    }  else {
        // parent goes down this path (main)
        printf("hello, I am parent of %d (pid:%d)\n",
               rc, (int) getpid());
    }
    return 0;
 }

 /*
  * Question 2a:
  * Beide können auf den Output file schreiben
  *
  * Question 2b:
  * Beide können gleichzeitig auf den File schreiben -> Parent kann als erstes kommen, aber auch Child
  */