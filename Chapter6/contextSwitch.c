#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <memory.h>

int main(int argc, char const *argv[])
{

    const int counter = 10000;
    const unsigned long billion = 1000000000;
    struct timespec startContextSwitch, stopContextSwitch;
    unsigned long sec[counter], nsec[counter],contextSwitchTime,loopTime;
    int nBytes;
    int pipeM[2];
    char* string = "Hallo, Test";
    char readBuffer[100];
    pid_t childPid;

    //-----------LOOP TIME--------------

    struct timespec startLoop, stopLoop;

    if (    clock_gettime(CLOCK_MONOTONIC_RAW, &startLoop) < 0) {
        printf("Start-Clock failed\n");
        exit(1);
    }

    for (int i = 0; i <= counter; i++) {
    }

    if (clock_gettime(CLOCK_MONOTONIC_RAW, &stopLoop) < 0 ) {
        printf("Stop-Clock failed\n");
        exit(1);
    }

    //-----------LOOP TIME--------------
    if (startLoop.tv_nsec > stopLoop.tv_nsec) {
        loopTime = (((stopLoop.tv_sec - 1) - startLoop.tv_sec) * billion)
                   + ((stopLoop.tv_nsec + billion) - startLoop.tv_nsec);
    } else {
        loopTime = (stopLoop.tv_sec - startLoop.tv_sec) + (stopLoop.tv_nsec - startLoop.tv_nsec);
    }

    unsigned long calcLoopTime = loopTime / counter;

    cpu_set_t  process;
    CPU_ZERO(&process);
    unsigned int length = sizeof(process);
    CPU_SET(0, &process);


    pipe(pipeM);

    if ((childPid = fork()) < 0) {
        fprintf(stderr, "fork failed\n");
        return 1;
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &startContextSwitch);
    if ((sched_setaffinity(0, length, &process)) < 1) {
        if (childPid == 0) {
            write(pipeM[0], string, strlen(string) + 1);
            nBytes = read(pipeM[1], readBuffer,sizeof(readBuffer));
            printf("Received String: %s\n", readBuffer);
            close(pipeM[0]);
            close(pipeM[1]);
            exit(0);
        } else {
            nBytes = read(pipeM[0],readBuffer,sizeof(readBuffer));
            write(pipeM[1], string, strlen(string) + 1);
            wait(0);
        }
    } else {
        perror("Error while setting sched_affinity");
        _exit(EXIT_FAILURE);
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &stopContextSwitch);

    if (startContextSwitch.tv_nsec > stopContextSwitch.tv_nsec) {
        contextSwitchTime = (((stopContextSwitch.tv_sec - 1) - startContextSwitch.tv_sec) * billion)
                         + ((stopContextSwitch.tv_nsec + billion) - startContextSwitch.tv_nsec);
    } else {
        contextSwitchTime = (stopContextSwitch.tv_sec - startContextSwitch.tv_sec) + (stopContextSwitch.tv_nsec - startContextSwitch.tv_nsec);
    }

    unsigned long calcTime = (contextSwitchTime / counter) - calcLoopTime;

    printf("\nOne Context-Switch takes %ld ns\n", calcTime);

    return 0;
}