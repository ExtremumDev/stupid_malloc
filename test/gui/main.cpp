#include <unistd.h>
#include <stdio.h>

#include "core.hpp"

int main()
{
    /* Simpled demonstration */
    StupidMallocTest t;

    void *new_all;

    for (int i = 0; i < 10; i++) {
        printf("Allcoation number %d\n", i);
        new_all = t.malloc(10);
        usleep(1500000);
    }
    return 0;
}