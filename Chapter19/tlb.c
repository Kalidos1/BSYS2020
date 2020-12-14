// Gruppe 18: Andre Göhring , Kira Koch
// Code Chapter 19 - tlb.c

#define _GNU_SOURCE
#include <sched.h>
#include <stdlib.h>
#include <sys/time.h> // Ist nötig für die Ausführung auf dem Container
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <memory.h>

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
        unsigned long tlbAccessTime;
        struct timespec startTLBAccess, stopTLBAccess;
        int jump;

        const int PAGESIZE = 4096; // Im Container mit -> "getconf PAGESIZE"
        jump = PAGESIZE / sizeof(int);


        cpu_set_t set;
        CPU_ZERO(&set);
        CPU_SET(4, &set); // Speziell für unseren Container -> //CPU 4,7-8,11 sind Möglich -> "cat /sys/fs/cgroup/cpuset/cpuset.cpus"
        if (sched_setaffinity(0, sizeof(set), &set)) {
            perror("error setting sched_affinity");
            _exit(EXIT_FAILURE);
        }

        printf("Number of Pages %i\n", NUMPAGES);
        printf("Number of Iterations %i\n", counter);

            long long totalTlbAccessTime = 0;

            for (int i = 1; i < counter; i++) {

                int* array = (int*) calloc((counter * NUMPAGES), sizeof(int)); //malloc

                if (array == NULL) {
                    fprintf(stderr, "Error allocating memory: ");
                    exit(EXIT_FAILURE);
                }

                for (int j = 1; j < NUMPAGES * jump; j += jump) {

                    //-----------TLB Call--------------
                    if (clock_gettime(CLOCK_MONOTONIC_RAW, &startTLBAccess) < 0) { //CLOCK_REALTIME/CLOCK_MONOTONIC gehen auch
                        printf("Start-Clock failed\n");
                        free(array);
                        exit(1);
                    }

                    array[j] += 1;

                    if (clock_gettime(CLOCK_MONOTONIC_RAW, &stopTLBAccess) < 0) {
                        printf("Stop-Clock failed\n");
                        free(array);
                        exit(1);
                    }

                    //-----------TLB Call Time--------------
                    // -> Umrechnung, falls erster Zeitpunkt < zweiter Zeitpunkt
                    if (startTLBAccess.tv_nsec > stopTLBAccess.tv_nsec) {
                        tlbAccessTime += (((stopTLBAccess.tv_sec - 1) - startTLBAccess.tv_sec) * billion)
                                        + ((stopTLBAccess.tv_nsec + billion) - startTLBAccess.tv_nsec);
                    } else {
                        // Berechnung der Zeit
                        tlbAccessTime += (stopTLBAccess.tv_sec - startTLBAccess.tv_sec) +
                                        (stopTLBAccess.tv_nsec - startTLBAccess.tv_nsec);
                    }

                }
                totalTlbAccessTime += tlbAccessTime / i; //könnte 0 sein!!!!
                free(array);
            }


            printf("tlbAccessTime: %ld\n", tlbAccessTime);
            unsigned long calcTime = (totalTlbAccessTime / counter);
            printf("\nOne TLB-Access takes %ld ns\n", calcTime);



    }
    return 0;
}
