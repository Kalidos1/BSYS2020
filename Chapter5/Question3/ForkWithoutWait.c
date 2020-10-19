#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int bool = 0;
    int rc = fork();
    if (rc < 0) {
        // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc==0) {
        // child (new process)
        printf("hello\n");
        bool = 1;
    }  else {
        // parent goes down this path (main)
        if (bool == 1) {
            printf("goodbye\n");
        }
    }
    return 0;
 }

 /*
  * Question 3a:
  * Keinen Weg gefunden der das ohne wait() macht ...
  * Variable setzen geht nicht, da beide Prozesse ihre "eigene" Variable haben
  * Man kann nicht gewährleisten ohne wait(), das beide in einer bestimmten Reihenfolge ablaufen
  *
  */