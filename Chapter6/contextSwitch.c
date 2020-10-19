//GruppenNR 5 - Goehring Andre,Marvin Klett
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <sched.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{

    long startPipe,stopPipe, testTime;
    struct timespec time;
    int nBytes;
    int pipeM[2];
    char* string = "Hallo, Test";
    char readBuffer[100];
    pid_t childPid;

    cpu_set_t  process;
    CPU_ZERO(&process);
    unsigned int length = sizeof(process);
    CPU_SET(0, &process);


    pipe(pipeM);

    if ((childPid = fork()) == -1) {
        perror("fork");
        exit(1);
    }
    clock_gettime(CLOCK_MONOTONIC, &time);
    startPipe = time.tv_nsec;
    if ((sched_setaffinity(0,length,&process)) < 1) {
        if (childPid == 0) {
            close(pipeM[1]);
            nBytes = read(pipeM[0],readBuffer,sizeof(readBuffer));
            printf("Received String: %s\n ", readBuffer);
            exit(0);
        } else {
            close(pipeM[0]);
            write(pipeM[1], string, strlen(string) + 1);
            gettimeofday(&time,NULL);
            stopPipe = time.tv_nsec;
            testTime = stopPipe - startPipe;
            printf("ContextSwitch Time: %ld \n", testTime);
            close(pipeM[1]);
            wait(0);
        }
    }

    return 0;
}