#include <stdio.h>
#include <stdlib.h>
#include "derive.h" 

void derive_fun1(struct base* b)
{
    struct derive *d = (struct derive *)b;
    d->x = 1;
    printf("This is derive function %d\n", d->x);
}

void derive_fun2(struct base* b)
{
    struct derive *d = (struct derive *)b;
    d->x = 2;
    printf("This is derive function %d\n", d->x);
}

void derive_fun3(struct base*b, char *str)
{
    struct derive *d = (struct derive *)b;
    d->x = 3;
    printf("This is derive function %d\n", d->x);
    printf("%s\n", str);
}

struct derive* derive_new()
{
    struct derive *d = malloc(sizeof(struct derive));    
    if (d == NULL) {
        perror("malloc");
        exit(1);
    }

    d->b.fun1 = derive_fun1;
    d->b.fun2 = derive_fun2;
    d->b.fun3 = derive_fun3;
    d->x = 10;
    return d;
}

void derive_destroy(struct derive* derive)
{
    if (derive != NULL) {
        free(derive);
        derive = NULL;
    }
}

