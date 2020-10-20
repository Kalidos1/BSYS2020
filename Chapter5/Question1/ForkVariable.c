#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    printf("hello world (pid:%d)\n", (int) getpid());
    int x = 100;
    int rc = fork();
    if (rc < 0) {
        // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc==0) {
        // child (new process)
        x = 50;
        printf("Child Variable X: %d\n", x);
        printf("hello, I am child (pid:%d)\n", (int) getpid());
    }  else {
        x = 150;
        // parent goes down this path (main)
        printf("Parent Variable X: %d\n", x);
        printf("hello, I am parent of %d (pid:%d)\n",
               rc, (int) getpid());
    }
    printf("Variable X: %d\n", x);
    return 0;
 }

 /*
  * Question 1a:
  * Wert von der Variable ist 100, wenn es vom Child Prozess aufgerufen wurde
  *
  * Question 1b:
  * Der Wert der Variable ist immer gleich in dem korrespondierenden Prozess (Child = 50, Parent = 150)
  * Wenn aber der Wert am Ende aufgerufen wird, kann es sein dass man nicht den erwartet Wert erhält
  * -> Die Variable gibt den Wert zurück, welcher Prozess die Variable als letztes aufgerufen hat, was hier in dem Fall noch beide Prozesse sein können
  */
