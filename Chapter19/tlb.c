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
        printf("No arguments given. Two Argument expected. \n Give your Number of Pages and the Number of Trials");
        return -1;
    }
    if (argc == 3) {
        const unsigned long NUMPAGES = atoi(argv[1]);
        const unsigned long counter = atoi(argv[2]);
        const unsigned long billion = 1000000000; //Für die Umrechnung -> Sekunde zu Nanosekunde (1 Milliarde)
        unsigned long systemCallTime, loopTime;
        struct timespec startSystemCall, stopSystemCall;
        int jump;
        int* array = (int*) malloc(counter * sizeof(int));
        const int PAGESIZE = 4096; // Im Container mit -> "getconf PAGESIZE"


        printf("Number of Pages %ld\n", NUMPAGES);
        printf("Number of Iterations %ld\n", counter);

        //-----------TLB Call--------------
        if (clock_gettime(CLOCK_MONOTONIC_RAW, &startSystemCall) < 0) { //CLOCK_REALTIME/CLOCK_MONOTONIC gehen auch
            printf("Start-Clock failed\n");                                        // -> Alle ca. 500ns
            exit(1);
        }

        jump = PAGESIZE / sizeof(int);
        for (int i = 0; i <= counter; i++) {
            for (int j = 0; j < NUMPAGES * jump; j += jump) {
                array[j] += 1;
            }
        }

        if (clock_gettime(CLOCK_MONOTONIC_RAW, &stopSystemCall) < 0) {
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
        if (startSystemCall.tv_nsec > stopSystemCall.tv_nsec) {
            systemCallTime = (((stopSystemCall.tv_sec - 1) - startSystemCall.tv_sec) * billion)
                             + ((stopSystemCall.tv_nsec + billion) - startSystemCall.tv_nsec);
        } else {
            // Berechnung der Zeit
            systemCallTime = (stopSystemCall.tv_sec - startSystemCall.tv_sec) +
                             (stopSystemCall.tv_nsec - startSystemCall.tv_nsec);
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

        printf("systemCallTime: %ld\n", systemCallTime);
        printf("loopTime: %ld\n", loopTime);
        unsigned long calcTime = (systemCallTime / counter) - (loopTime / counter);
        printf("\nOne System-Call takes %ld ns\n", calcTime);

        return 0;
    }
}
