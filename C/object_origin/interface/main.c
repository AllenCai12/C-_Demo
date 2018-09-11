#include "derive.h" 

void do_something( struct base *b)
{
    b->fun1(b);
    b->fun2(b);
    b->fun3(b, "hello world");
}

int main(int argc, char **argv)
{
    struct derive *de = derive_new();
    do_something((struct base*)de); 
    derive_destroy(de);
    return 0;
}
