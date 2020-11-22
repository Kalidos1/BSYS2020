#include <stdio.h>
#include <sys/time.h> // Ist nötig für die Ausführung auf dem Container
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    if (argc > 3) {
        printf("To many arguments");
        return -1;
    }
    if (argc < 2) {
        printf("No arguments given. Two Argument expected. \n Give your Number of Pages and the Number of Trials\n");
        return -1;
    }
    if (argc == 3) {
        const int NUMPAGES = atoi(argv[1]);
        const int counter = atoi(argv[2]);
        const unsigned long billion = 1000000000; //Für die Umrechnung -> Sekunde zu Nanosekunde (1 Milliarde)
        unsigned long tlbAccessTime, loopTime;
        struct timespec startTLBAccess, stopTLBAccess;
        int jump;
        int* array = (int*) malloc(counter * sizeof(int));
        const int PAGESIZE = 4096; // Im Container mit -> "getconf PAGESIZE"


        printf("Number of Pages %i\n", NUMPAGES);
        printf("Number of Iterations %i\n", counter);

        //-----------TLB Call--------------
        if (clock_gettime(CLOCK_MONOTONIC_RAW, &startTLBAccess) < 0) { //CLOCK_REALTIME/CLOCK_MONOTONIC gehen auch
            printf("Start-Clock failed\n");                                        // -> Alle ca. 500ns
            exit(1);
        }

        jump = PAGESIZE / sizeof(int);
        for (int i = 0; i <= counter; i++) {
            for (int j = 0; j < NUMPAGES * jump; j += jump) {
                array[j] += 1;
            }
        }

        if (clock_gettime(CLOCK_MONOTONIC_RAW, &stopTLBAccess) < 0) {
            printf("Stop-Clock failed\n");
            exit(1);
        }

        //-----------LOOP TIME--------------

        struct timespec startLoop, stopLoop;

        if (clock_gettime(CLOCK_MONOTONIC_RAW, &startLoop) < 0) {
            printf("Start-Clock failed\n");
            exit(1);
        }

        for (int i = 0; i <= counter; i++) {
        }

        if (clock_gettime(CLOCK_MONOTONIC_RAW, &stopLoop) < 0) {
            printf("Stop-Clock failed\n");
            exit(1);
        }

        //-----------SYSTEM CALL TIME--------------
        // -> Umrechnung, falls erster Zeitpunkt < zweiter Zeitpunkt
        if (startTLBAccess.tv_nsec > stopTLBAccess.tv_nsec) {
            tlbAccessTime = (((stopTLBAccess.tv_sec - 1) - startTLBAccess.tv_sec) * billion)
                            + ((stopTLBAccess.tv_nsec + billion) - startTLBAccess.tv_nsec);
        } else {
            // Berechnung der Zeit
            tlbAccessTime = (stopTLBAccess.tv_sec - startTLBAccess.tv_sec) +
                            (stopTLBAccess.tv_nsec - startTLBAccess.tv_nsec);
        }

        //-----------LOOP TIME--------------
        // -> Umrechnung, falls erster Zeitpunkt < zweiter Zeitpunkt
        if (startLoop.tv_nsec > stopLoop.tv_nsec) {
            loopTime = (((stopLoop.tv_sec - 1) - startLoop.tv_sec) * billion)
                       + ((stopLoop.tv_nsec + billion) - startLoop.tv_nsec);
        } else {
            // Berechnung der Zeit
            loopTime = (stopLoop.tv_sec - startLoop.tv_sec) + (stopLoop.tv_nsec - startLoop.tv_nsec);
        }

        printf("tlbAccessTime: %ld\n", tlbAccessTime);
        printf("loopTime: %ld\n", loopTime);
        unsigned long calcTime = (tlbAccessTime / counter) - (loopTime / counter);
        printf("\nOne TLB-Access takes %ld ns\n", calcTime);

        return 0;
    }
}
