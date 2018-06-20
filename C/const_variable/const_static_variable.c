#include <stdio.h>

struct test{
    char *pa;
    char *pb;
};

static const struct test t[10];

int main(int argc, char **argv)
{

    int i;
    char a = 'b';
    for (i=0; i < 10; i++) {
        printf("pa[%d] = %p\n", i, t[i].pa);
        printf("pb[%d] = %p\n", i, t[i].pb);
        t[i].pa = &a;
    }
    

    return 0;
}
