#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include "common_threads.h"

//
// Here, you have to write (almost) ALL the code. Oh no!
// How can you show that a thread does not starve
// when attempting to acquire this mutex you build?
//

typedef struct __ns_mutex_t {
    int counter;
    sem_t s1, s2;
} ns_mutex_t;

void ns_mutex_init(ns_mutex_t *m) {
    sem_init(&m->s1, 0 , 1);
    sem_init(&m->s2, 0 , 1);
    m->counter = 0;
}

void ns_mutex_acquire(ns_mutex_t *m) {
    int tid = syscall(SYS_gettid);
    if ((m->counter % tid) == 0) {
        sem_wait(&m->s1);
    } else {
        sem_wait(&m->s2);
        sem_wait(&m->s1);
    }
    sem_wait(&m->s1);
    printf("%d\n",tid);
}

void ns_mutex_release(ns_mutex_t *m) {
    m->counter++;
    sem_post(&m->s1);
    sem_post(&m->s2);
    sleep(1);
}


void *worker(void *arg) {
    for (int i = 0; i < 100; i++) {
        ns_mutex_acquire(arg);
        printf("Test\n");
        ns_mutex_release(arg);
    }
    return NULL;
}

ns_mutex_t mutex;

int main(int argc, char *argv[]) {
    pthread_t p1, p2;
    printf("parent: begin\n");
    ns_mutex_init(&mutex);
    Pthread_create(&p1, NULL, worker, &mutex);
    Pthread_create(&p2, NULL, worker, &mutex);
    Pthread_join(p1, NULL);
    Pthread_join(p2, NULL);
    printf("parent: end\n");
    return 0;
}

