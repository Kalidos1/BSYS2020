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
        int wc = wait(NULL);
        printf("hello, I am child (wc:%d) (pid:%d)\n", wc, (int) getpid());
    }  else {
        // parent goes down this path (main)
        printf("hello, I am parent of %d (pid:%d)\n",
               rc, (int) getpid());
    }
    return 0;
 }

 /*
  * Question 5a:
  * andre@Andres-MBP Question5 % ./fork5
  * hello, I am child (pid:6378)
  * hello, I am parent of 6378 (wc:6378) (pid:6377)
  * => The wait() Method returns the PID of the Child Process on which the parent waits on
  *
  * Question 5b:
  * andre@Andres-MBP Question5 % ./fork5
  * hello, I am parent of 6402 (pid:6401)
  * hello, I am child (wc:-1) (pid:6402)
  * => The wait() Method returns -1, but the Parent does come before the Child, so the child waits until the parent is finished
  */