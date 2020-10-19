#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    const int counter = 1000000;
    const unsigned long billion = 1000000000;
    unsigned long systemCallTime,loopTime;
    struct timespec startSystemCall, stopSystemCall;

    //-----------SYSTEM CALL--------------
    if (clock_gettime(CLOCK_MONOTONIC_RAW, &startSystemCall) < 0) {
        printf("Start-Clock failed\n");
        exit(1);
    }

    for (int i = 0; i <= counter; i++) {
        getpid();
    }

    if (clock_gettime(CLOCK_MONOTONIC_RAW, &stopSystemCall) < 0 ) {
        printf("Stop-Clock failed\n");
        exit(1);
    }

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

    //-----------SYSTEM CALL TIME--------------
    if (startSystemCall.tv_nsec > stopSystemCall.tv_nsec) {
        systemCallTime = (((stopSystemCall.tv_sec - 1) - startSystemCall.tv_sec) * billion)
                + ((stopSystemCall.tv_nsec + billion) - startSystemCall.tv_nsec);
    } else {
        systemCallTime = (stopSystemCall.tv_sec - startSystemCall.tv_sec) + (stopSystemCall.tv_nsec - startSystemCall.tv_nsec);
    }

    //-----------LOOP TIME--------------
    if (startLoop.tv_nsec > stopLoop.tv_nsec) {
        loopTime = (((stopLoop.tv_sec - 1) - startLoop.tv_sec) * billion)
                         + ((stopLoop.tv_nsec + billion) - startLoop.tv_nsec);
    } else {
        loopTime = (stopLoop.tv_sec - startLoop.tv_sec) + (stopLoop.tv_nsec - startLoop.tv_nsec);
    }

    printf("systemCallTime: %ld\n", systemCallTime);
    printf("loopTime: %ld\n", loopTime);
    unsigned long calcTime = (systemCallTime / counter) - (loopTime / counter);
    printf("\nOne System-Call takes %ld ns\n", calcTime);

    return 0;
}
