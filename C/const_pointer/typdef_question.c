#include <stdio.h>

struct Test{
    int a;
    int b;
};

typedef struct Test* T;
typedef struct Test t;
/*记住const t* 和 const T是不一样的
**一个是常量指针, 另一个是指针常量
*/
void test(const T tp)
{
    tp->a = 5;
    tp->b = 6;
    printf("test start...\n");
    printf("a=%d, b=%d\n", tp->a, tp->b);
    printf("test end...\n");
    
}

int main(int argc, char **argv)
{
    struct Test t = {0};    
    T tp = &t;

    printf("a=%d, b=%d\n", t.a, t.b);

    test(tp);

    printf("a=%d, b=%d\n", t.a, t.b);
    return 0;
}
