#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int* array = (int*) malloc(10 * sizeof(int));
    printf("Adress of Pointer: %i\n", array);
    free(array);
    free(array);
    return 0;
}

//Error nur mit Valgrind, Programm läuft durch, gdb zeigt nichts