#include "Atom.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    Atom_int(12456);

    printf("hello world\n");

    int i = 10;
    while(i--) {
        sleep(1);
    }

    return 0;
}