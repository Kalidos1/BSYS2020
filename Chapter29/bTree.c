// Gruppe 18: Andre Göhring , Kira Koch
// Code Chapter 29 - ...



// QUELLE: https://www.programiz.com/dsa/b-tree
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
#define MAX 3
#define MIN 2

//--------------FROM SOURCE: https://www.programiz.com/dsa/b-tree -----------------------
//--------------Modified with Locks
struct BTreeNode {
    int val[MAX + 1], count;
    struct BTreeNode *link[MAX + 1];
    pthread_mutex_t lock;
};

struct BTreeNode *root;

// Create a node
struct BTreeNode *createNode(int val, struct BTreeNode *child) {
    struct BTreeNode *newNode;
    newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    newNode->val[1] = val;
    newNode->count = 1;
    newNode->link[0] = root;
    newNode->link[1] = child;
    pthread_mutex_init(&newNode->lock, NULL);
    return newNode;
}

// Insert node
void insertNode(int val, int pos, struct BTreeNode *node,
                struct BTreeNode *child) {
    int j = node->count;
    while (j > pos) {
        node->val[j + 1] = node->val[j];
        node->link[j + 1] = node->link[j];
        j--;
    }
    node->val[j + 1] = val;
    node->link[j + 1] = child;
    node->count++;
}

// Split node
void splitNode(int val, int *pval, int pos, struct BTreeNode *node,
               struct BTreeNode *child, struct BTreeNode **newNode) {
    int median, j;

    if (pos > MIN)
        median = MIN + 1;
    else
        median = MIN;

    *newNode = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    j = median + 1;
    while (j <= MAX) {
        (*newNode)->val[j - median] = node->val[j];
        (*newNode)->link[j - median] = node->link[j];
        j++;
    }
    node->count = median;
    (*newNode)->count = MAX - median;

    if (pos <= MIN) {
        insertNode(val, pos, node, child);
    } else {
        insertNode(val, pos - median, *newNode, child);
    }
    *pval = node->val[node->count];
    (*newNode)->link[0] = node->link[node->count];
    node->count--;
}

// Set the value
int setValue(int val, int *pval,
             struct BTreeNode *node, struct BTreeNode **child) {
    int pos;
    if (!node) {
        *pval = val;
        *child = NULL;
        return 1;
    }

    if (val < node->val[1]) {
        pos = 0;
    } else {
        for (pos = node->count;
             (val < node->val[pos] && pos > 1); pos--)
            ;
        if (val == node->val[pos]) {
            printf("Duplicates are not permitted\n");
            return 0;
        }
    }
    if (setValue(val, pval, node->link[pos], child)) {
        if (node->count < MAX) {
            pthread_mutex_lock(&node->lock);
            insertNode(*pval, pos, node, *child);
            pthread_mutex_unlock(&node->lock);
        } else {
            pthread_mutex_lock(&node->lock);
            splitNode(*pval, pval, pos, node, *child, child);
            pthread_mutex_unlock(&node->lock);
            return 1;
        }
    }
    return 0;
}

// Insert the value
void insert(int val) {
    int flag, i;
    struct BTreeNode *child;

    flag = setValue(val, &i, root, &child);
    if (flag)
        root = createNode(i, child);
}

// Search node
void search(int val, int *pos, struct BTreeNode *myNode) {
    if (!myNode) {
        return;
    }

    if (val < myNode->val[1]) {
        *pos = 0;
    } else {
        for (*pos = myNode->count;
             (val < myNode->val[*pos] && *pos > 1); (*pos)--)
            ;
        if (val == myNode->val[*pos]) {
            printf("%d is found", val);
            return;
        }
    }
    pthread_mutex_lock(&myNode->lock);
    search(val, pos, myNode->link[*pos]);
    pthread_mutex_unlock(&myNode->lock);

    return;
}

// Traverse then nodes
void traversal(struct BTreeNode *myNode) {
    int i;
    if (myNode) {
        for (i = 0; i < myNode->count; i++) {
            traversal(myNode->link[i]);
            printf("%d ", myNode->val[i + 1]);
        }
        traversal(myNode->link[i]);
    }
}

//--------------------------------------------------------------



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

        insert(i);

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
        int ch;
        const int increment = atoi(argv[1]);
        const int threads = atoi(argv[2]);
        pthread_t thread[threads];
        printf("Number of Increments by each Thread:  %i\n", increment);
        printf("Number of Threads:  %i\n", threads);

        //BTree_INIT

        for (int i = 0; i < threads; i++) {
            Pthread_create(&thread[i], NULL, worker, (void *) (long long) increment);
        }

        for (int i = 0; i < threads; i++) {
            Pthread_join(thread[i], NULL);
        }

        //printf("Counter is: %d\n", search(increment, &ch, root));
        double accessTimeSeconds = (double) sloppyCounterAccessTime / 1e9;
        printf("sloppyCounterAccessTime: %ld ns\n", sloppyCounterAccessTime);
        printf("sloppyCounterAccessTime in Sekunden: %f s\n", (double) accessTimeSeconds);
        //unsigned long calcTime = (sloppyCounterAccessTime / 4);
        //printf("\nOne Counter-Access takes %ld ns\n", calcTime);

    }

    return 0;
}

