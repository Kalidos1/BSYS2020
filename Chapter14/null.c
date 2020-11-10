#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int* p;
    int value;

    p = NULL;

    value = *p;
    printf("%d", value);

    return 0;
}

// Error auch ohne gdb/Valgrind