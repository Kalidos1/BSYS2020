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
        char* array;
        unsigned long i;

        array = (char*) malloc(megabytes * sizeof(char));

        if (array == NULL) {
            printf("malloc of size %ld failed", megabytes);
        }

        for(i = 0; i < megabytes + 1; i++) {
<<<<<<< HEAD
            array[i] = 1;
=======
            array[i] = 'a';

            printf("%i", array[i]);

>>>>>>> 182e0676178f78e857bc3bcef4a2716c353e96ef
            if (i == megabytes) {
                i = 0;
            }
        }

        free(array);
    }

    return 0;
}

//Did it with Int first but int does allocate 2-4 Byte per int, so it does result in more Allocated MEM
