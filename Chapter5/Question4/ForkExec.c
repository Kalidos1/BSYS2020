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
  * Jede Exec() Methode funktioniert
  * execve() and execvP() brauchen 3 Parameter => 1. argv, 2. searchpath, aber mit diesen funktioniert es auch
  * Alle anderen, die 2 Parameter brauchen, funktionieren so wie sie es sollen
  * Es gibt viele verschiedene Exec() calls für:
  * -> Unterschiedliche Anfänge der Programme
  * -> Unterschiedliche Ziele
  * -> Von der CMD-Line Dinge übergeben mit execve() => Argv
  * ->
  *
  */