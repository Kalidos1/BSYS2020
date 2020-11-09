#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int* data = (int*) malloc(100 * sizeof(int));

    free(&data[20]);

    printf("Value: %i\n", data[10]);
    return 0;
}