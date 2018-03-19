#include "Atom.h"
#include <stdio.h>
#include <assert.h>

const char *Atom_new(const char *str, int length)
{

    return NULL;
}

int Atom_length(const char *str)
{
    return 0;
}

const char *Atom_int(long n)
{
    char str[46] = {0};
    long m;

    if (n < 0) {
        m = -n;
    }
    else {
        m = n;
    }

    long num = m;
    int i;

    for (i = sizeof(str) -1; m != 0 && i > 2; m = m /10, i--) {
        str[i] = (char)('0' + (m % 10));
        printf("str[%d] = %c  \n", i, str[i]);
    }

    if (n < 0) {
        str[i] = '-';
        i++;
    }

    str[i] = '0';

    printf("str = ");
    for (i=sizeof(str)-1; str[i] != '0'; i--) {
        printf("%c", str[i]);
    }

    return  NULL;
}

const char *Atom_string(const char *str, int length)
{
    assert(str);

    return Atom_new(str, length);
}
