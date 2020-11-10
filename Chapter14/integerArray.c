#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int* data = (int*) malloc(100 * sizeof(int));

    data[100] = 0;
    return 0;
}

//Error nur mit Valgrind, Programm läuft durch, gdb zeigt nichts