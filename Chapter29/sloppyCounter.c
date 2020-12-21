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

#include "mythreads.h"

const int NUMCPUS = 4;

typedef struct __counter_t {
    int global;
    pthread_mutex_t glock;
    int local[NUMCPUS];
    pthread_mutex_t llock[NUMCPUS];
    int threshold;
} counter_t;

// init: record threshold, init locks, init values
//       of all local counts and global count
void init(counter_t *c, int threshold) {
    c->threshold = threshold;
    c->global = 0;
    pthread_mutex_init(&c->glock, NULL);
    int i;
    for (i = 0; i < NUMCPUS; i++) {
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


counter_t c;
const unsigned long billion = 1000000000; //Für die Umrechnung -> Sekunde zu Nanosekunde (1 Milliarde)
unsigned long counterAccessTime = 0;
struct timespec startCounterAccess, stopCounterAccess;

void *worker(void *arg) {
    for (int i = 0; i < (long long) arg; i++) {
        // Lock zwischen den Zeit-Messungen ??????
        if (clock_gettime(CLOCK_MONOTONIC_RAW, &startCounterAccess) < 0) { //CLOCK_REALTIME/CLOCK_MONOTONIC gehen auch
            printf("Start-Clock failed\n");
            exit(1);
        }

        update(&c, (unsigned long int) pthread_self() ,1);
        if (clock_gettime(CLOCK_MONOTONIC_RAW, &stopCounterAccess) < 0) {
            printf("Stop-Clock failed\n");
            exit(1);
        }
        //----------Access Time--------------
        // -> Umrechnung, falls erster Zeitpunkt < zweiter Zeitpunkt
        if (startCounterAccess.tv_nsec > stopCounterAccess.tv_nsec) {
            counterAccessTime += (((stopCounterAccess.tv_sec - 1) - startCounterAccess.tv_sec) * billion)
                                 + ((stopCounterAccess.tv_nsec + billion) - startCounterAccess.tv_nsec);
        } else {
            // Berechnung der Zeit
            counterAccessTime += (stopCounterAccess.tv_sec - startCounterAccess.tv_sec) +
                                 (stopCounterAccess.tv_nsec - startCounterAccess.tv_nsec);
        }
    }
    return NULL;
}

int main(int argc, char const *argv[]) {
    if (argc > 2) {
        printf("To many arguments");
        return -1;
    }
    if (argc < 1) {
        printf("No arguments given. One Argument expected. \n Give your Number of increments by each thread\n");
        return -1;
    }
    const int increment = atoi(argv[1]);
    printf("Number of Increments by each Thread:  %i\n", increment);

    init(&c, 2);

    pthread_t p1, p2, p3, p4;
    Pthread_create(&p1, NULL, worker, (void *) (long long) increment);
    Pthread_create(&p2, NULL, worker, (void *) (long long) increment);
    Pthread_create(&p3, NULL, worker, (void *) (long long) increment);
    Pthread_create(&p4, NULL, worker, (void *) (long long) increment);
    //Pthread_create(&p5, NULL, worker, (void *) (long long) increment);


    Pthread_join(p1, NULL);
    Pthread_join(p2, NULL);
    Pthread_join(p3, NULL);
    Pthread_join(p4, NULL);
    //Pthread_join(p5, NULL);

    printf("Counter is: %d\n", get(&c));
    double accessTimeSeconds = (double) counterAccessTime / 1e9;
    printf("counterAccessTime: %ld ns\n", counterAccessTime);
    printf("counterAccessTime in Sekunden: %f s\n", (double) accessTimeSeconds);
    //unsigned long calcTime = (counterAccessTime / 4);
    //printf("\nOne Counter-Access takes %ld ns\n", calcTime);

    return 0;
}

