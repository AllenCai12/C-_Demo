#include <stdio.h>


void on_show(int a)
{
    printf("a:%d\n", a);
}

struct callback {
    void (*on_test)(int a);
};

void test(struct callback *callback)
{
    callback->on_test(123);
}

int main(int argc, char **argv)
{
    struct callback callback;
    callback.on_test = on_show(123);
    test(&callback);   
    return 0;
}
