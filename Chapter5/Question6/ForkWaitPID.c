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
        printf("hello, I am child (pid:%d)\n", (int) getpid());
    }  else {
        // parent goes down this path (main)
        int wc = waitpid(-1, NULL, WUNTRACED);
        printf("hello, I am parent of %d (wc:%d) (pid:%d)\n",
               rc, wc, (int) getpid());
    }
    return 0;
 }

 /*
  * Question 6a:
  * waitpid() könnte nutzvoll sein, wenn man auf bestimmte Child Prozesse warten will,
  * oder auf irgendeinen Child Prozess der erstellelt wurde
  */