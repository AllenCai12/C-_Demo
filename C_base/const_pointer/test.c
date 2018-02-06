#include <stdio.h>


int main(int argc, char **argv)
{
    char a = 'h';
    char b = 'c';

    char *pc = NULL;

    //常量指针，指向常量的指针 
#if 0
    const char *p = &a;
    char const *p = &a;
    p = &b; //正确， 常量指针， 可更改指针变量存储的地址
    *p = 'j'; //错误， 常量指针,不可更改指针变量地址存储的内容
#endif

    //指针常量，指针是一个常量
#if 1
    char * const p = &a;
//    p = &b; //错误， 指针常量， 不可更改指针变量存储的地址
    *p = 'j'; //正确， 指针常量,可更改指针变量地址存储的内容

#endif


    printf("p = %c\n", *p);
    return 0;
}
