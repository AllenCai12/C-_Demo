#ifndef C_DERIVE_H
#define C_DERIVE_H
#include "base.h"

struct derive {
    struct base b;
    int x;
};

struct derive* derive_new();
void derive_destroy(struct derive* derive);
void derive_fun1(struct base* b);
void derive_fun2(struct base* b);
void derive_fun3(struct base*b, char *str);
#endif
