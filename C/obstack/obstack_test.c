#include <stdio.h>
#include <obstack.h>
#include <stdlib.h>
#include <string.h>

#define obstack_chunk_alloc malloc
#define obstack_chunk_free free

int main(int argc, char **argv)
{
    char *test;
    char *test2;
    struct obstack *my_stack;
    my_stack = (struct obstack *)malloc(sizeof(struct obstack));

    obstack_init(my_stack);

    test = (char *)obstack_alloc(my_stack, 1000000);
    if (test == NULL) {
        perror("obstack alloc");
        exit(-1); 
    }

    test2 = (char *)obstack_alloc(my_stack, 1000000);
    if (test == NULL) {
        perror("obstack alloc");
        exit(-1); 
    }

    strcpy(test, "hello world");
    strcpy(test2, " 2 hello world");
    printf("%s\n", test);
    printf("%s\n", test2);

    /*按照栈的原则， 必须后进，先出*/ 
    obstack_free(my_stack, test2);
    obstack_free(my_stack, test);

    free(my_stack);


    return 0;
}
