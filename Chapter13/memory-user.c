#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc > 2) {
        printf("To many arguments");
        return -1;
    }
    if (argc < 2) {
        printf("No arguments given. One Argument expected. \n Give your wanted Byte-Number");
        return -1;
    }
    if (argc == 2) {
        unsigned long million = 1000000; //Megabytes
        unsigned long megabytes = (atoi(argv[1]) * million);
        int* array;
        unsigned long i;

        array = (int*) malloc(megabytes * sizeof(int));

        if (array == NULL) {
            printf("malloc of size %ld failed", megabytes);
        }

        for(i = 0; i < megabytes + 1; i++) {
            array[i] = 1;

            if (i == megabytes) {
                i = 0;
            }
        }

        free(array);
    }

    return 0;
}
