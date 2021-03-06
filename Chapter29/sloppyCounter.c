// Gruppe 18: Andre Göhring , Kira Koch
// Code Chapter 29 - ...

#define _GNU_SOURCE

#include <sched.h>
#include <stdlib.h>
#include <sys/time.h> // Ist nötig für die Ausführung auf dem Container
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <memory.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

#include "mythreads.h"

typedef struct __counterSloppy_t {
    int global;
    pthread_mutex_t glock;
    int local[4];
    pthread_mutex_t llock[4];
    int threshold;
} counter_t;

// init: record threshold, init locks, init values
//       of all local counts and global count
void init(counter_t *c, int threshold) {
    c->threshold = threshold;
    c->global = 0;
    pthread_mutex_init(&c->glock, NULL);
    int i;
    for (i = 0; i < 4; i++) {
        c->local[i] = 0;
        pthread_mutex_init(&c->llock[i], NULL);
    }
}

// update: usually, just grab local lock and update local amount
//         once local count has risen by ’threshold’, grab global
// lock and transfer local values to it
void update(counter_t *c, int threadID, int amt) {
    pthread_mutex_lock(&c->llock[threadID]);
    c->local[threadID] += amt;   // assumes amt > 0
    if (c->local[threadID] >= c->threshold) { // transfer to global
        pthread_mutex_lock(&c->glock);
        c->global += c->local[threadID];
        pthread_mutex_unlock(&c->glock);
        c->local[threadID] = 0;
    }
    pthread_mutex_unlock(&c->llock[threadID]);
}
// get: just return global amount (which may not be perfect)
int get(counter_t *c) {
    pthread_mutex_lock(&c->glock);
    int val = c->global;
    pthread_mutex_unlock(&c->glock);
    return val; // only approximate!
}


counter_t cSloppy;
const unsigned long billion = 1000000000; //Für die Umrechnung -> Sekunde zu Nanosekunde (1 Milliarde)
unsigned long sloppyCounterAccessTime = 0;
struct timespec startSloppyAccess, stopSloppyAccess;

void *worker(void *arg) {
    for (int i = 0; i < (long long) arg; i++) {
        pid_t x = syscall(SYS_gettid);
        // Lock zwischen den Zeit-Messungen ??????
        if (clock_gettime(CLOCK_MONOTONIC_RAW, &startSloppyAccess) < 0) { //CLOCK_REALTIME/CLOCK_MONOTONIC gehen auch
            printf("Start-Clock failed\n");
            exit(1);
        }

        update(&cSloppy, x, 1);
        if (clock_gettime(CLOCK_MONOTONIC_RAW, &stopSloppyAccess) < 0) {
            printf("Stop-Clock failed\n");
            exit(1);
        }
        //----------Access Time--------------
        // -> Umrechnung, falls erster Zeitpunkt < zweiter Zeitpunkt
        if (startSloppyAccess.tv_nsec > stopSloppyAccess.tv_nsec) {
            sloppyCounterAccessTime += (((stopSloppyAccess.tv_sec - 1) - startSloppyAccess.tv_sec) * billion)
                                 + ((stopSloppyAccess.tv_nsec + billion) - startSloppyAccess.tv_nsec);
        } else {
            // Berechnung der Zeit
            sloppyCounterAccessTime += (stopSloppyAccess.tv_sec - startSloppyAccess.tv_sec) +
                                 (stopSloppyAccess.tv_nsec - startSloppyAccess.tv_nsec);
        }
    }
    return NULL;
}

int main(int argc, char const *argv[]) {
    if (argc > 3) {
        printf("To many arguments");
        return -1;
    }
    if (argc < 2) {
        printf("No arguments given. Two Arguments expected. \n Give your Number of increments by each thread and your number of Threads\n");
        return -1;
    }
    if (argc == 3) {
        const int increment = atoi(argv[1]);
        const int threads = atoi(argv[2]);
        pthread_t thread[threads];
        printf("Number of Increments by each Thread:  %i\n", increment);
        printf("Number of Threads:  %i\n", threads);

        init(&cSloppy, 1024);

        for (int i = 0; i < threads; i++) {
            Pthread_create(&thread[i], NULL, worker, (void *) (long long) increment);
        }

        for (int i = 0; i < threads; i++) {
            Pthread_join(thread[i], NULL);
        }

        printf("Counter is: %d\n", get(&cSloppy));
        double accessTimeSeconds = (double) sloppyCounterAccessTime / 1e9;
        printf("sloppyCounterAccessTime: %ld ns\n", sloppyCounterAccessTime);
        printf("sloppyCounterAccessTime in Sekunden: %f s\n", (double) accessTimeSeconds);
        //unsigned long calcTime = (sloppyCounterAccessTime / 4);
        //printf("\nOne Counter-Access takes %ld ns\n", calcTime);

    }

    return 0;
}

