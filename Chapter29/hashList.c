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

#define BUCKETS (101)

typedef struct __node_t {
    int key;
    struct __node_t *next;
} node_t;

typedef struct __list_t {
    node_t *head;
    pthread_mutex_t lock;
} list_t;

typedef struct __hash_t {
    list_t lists[BUCKETS];
} hash_t;

void List_Init(list_t *L) {
    L->head = NULL;
    pthread_mutex_init(&L->lock, NULL);
}

int List_Insert(list_t *L, int key) {
    node_t *new = malloc(sizeof(node_t));
    if (new == NULL) {
        perror((const char *) malloc);
        pthread_mutex_unlock(&L->lock);
        return -1; // fail
    }
    new->key = key;
    pthread_mutex_lock(&L->lock);
    new->next = L->head;
    L->head = new;
    pthread_mutex_unlock(&L->lock);
    return 0; // success
}

int List_Lookup(list_t *L, int key) {
    int rv = -1;
    pthread_mutex_lock(&L->lock);
    node_t *curr = L->head;
    while (curr) {
        if (curr->key == key) {
            rv = 0;
            return 0; // success
        }
        curr = curr->next;
    }
    pthread_mutex_unlock(&L->lock);
    return rv; // failure
}

void Hash_Init(hash_t *H) {
    int i;
    for (i = 0; i < BUCKETS; i++) {
        List_Init(&H->lists[i]);
    }
}
int Hash_Insert(hash_t *H, int key) {
    int bucket = key % BUCKETS;
    return List_Insert(&H->lists[bucket], key);
}
int Hash_Lookup(hash_t *H, int key) {
    int bucket = key % BUCKETS;
    return List_Lookup(&H->lists[bucket], key);
}


hash_t hash;
const unsigned long billion = 1000000000; //Für die Umrechnung -> Sekunde zu Nanosekunde (1 Milliarde)
unsigned long sloppyCounterAccessTime = 0;
struct timespec startSloppyAccess, stopSloppyAccess;

void *worker(void *arg) {
    for (int i = 0; i < (long long) arg; i++) {
        // Lock zwischen den Zeit-Messungen ??????
        if (clock_gettime(CLOCK_MONOTONIC_RAW, &startSloppyAccess) < 0) { //CLOCK_REALTIME/CLOCK_MONOTONIC gehen auch
            printf("Start-Clock failed\n");
            exit(1);
        }

        Hash_Insert(&hash, i);
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

        Hash_Init(&hash);

        for (int i = 0; i < threads; i++) {
            Pthread_create(&thread[i], NULL, worker, (void *) (long long) increment);
        }

        for (int i = 0; i < threads; i++) {
            Pthread_join(thread[i], NULL);
        }

        printf("Counter is: %d\n", Hash_Lookup(&hash, increment * threads));
        double accessTimeSeconds = (double) sloppyCounterAccessTime / 1e9;
        printf("sloppyCounterAccessTime: %ld ns\n", sloppyCounterAccessTime);
        printf("sloppyCounterAccessTime in Sekunden: %f s\n", (double) accessTimeSeconds);
        //unsigned long calcTime = (sloppyCounterAccessTime / 4);
        //printf("\nOne Counter-Access takes %ld ns\n", calcTime);

    }

    return 0;
}

