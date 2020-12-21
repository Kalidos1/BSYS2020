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

typedef struct __counter_t {
    int value;
    pthread_mutex_t lock;
} counter_t;

void init(counter_t *c) {
    c->value = 0;
    Pthread_mutex_init((pthread_mutex_t *) &c->lock, NULL);
}

void increment(counter_t *c) {
    Pthread_mutex_lock(&c->lock);
    c->value++;
    Pthread_mutex_unlock(&c->lock);
}

void decrement(counter_t *c) {
    Pthread_mutex_lock(&c->lock);
    c->value --;
    Pthread_mutex_unlock(&c->lock);
}

int get(counter_t *c) {
    Pthread_mutex_lock(&c->lock);
    int rc = c->value;
    Pthread_mutex_unlock(&c->lock);
    return rc;
}

counter_t c;
const unsigned long billion = 1000000000; //Für die Umrechnung -> Sekunde zu Nanosekunde (1 Milliarde)
unsigned long counterAccessTime = 0;
struct timespec startCounterAccess, stopCounterAccess;

void* worker(void* arg) {
    for (int i = 0; i < (long long) arg; i++) {
        // Lock zwischen den Zeit-Messungen ??????
        if (clock_gettime(CLOCK_MONOTONIC_RAW, &startCounterAccess) < 0) { //CLOCK_REALTIME/CLOCK_MONOTONIC gehen auch
            printf("Start-Clock failed\n");
            exit(1);
        }

        increment(&c);
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

    init(&c);

    pthread_t p1, p2, p3, p4, p5;
    Pthread_create(&p1, NULL, worker, (void *) (long long) increment);
    Pthread_create(&p2, NULL, worker, (void *) (long long) increment);
    Pthread_create(&p3, NULL, worker, (void *) (long long) increment);
    Pthread_create(&p4, NULL, worker, (void *) (long long) increment);
    Pthread_create(&p5, NULL, worker, (void *) (long long) increment);


    Pthread_join(p1, NULL);
    Pthread_join(p2, NULL);
    Pthread_join(p3, NULL);
    Pthread_join(p4, NULL);
    Pthread_join(p5, NULL);

    get(&c);

    printf("Counter is: %d\n", c.value);
    double accessTimeSeconds = (double) counterAccessTime / 1e9;
    printf("counterAccessTime: %ld ns\n", counterAccessTime);
    printf("counterAccessTime in Sekunden: %f s\n", (double) accessTimeSeconds);
    //unsigned long calcTime = (counterAccessTime / 4);
    //printf("\nOne Counter-Access takes %ld ns\n", calcTime);

    return 0;
}

