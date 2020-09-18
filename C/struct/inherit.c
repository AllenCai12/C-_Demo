#include <stdio.h>

struct human {
    int age; 
};


struct white_human {
    struct human human;
};

struct black_human {
    struct human human;
};

void _show_human_age(struct human *human)
{
    printf("human age: %d\n", human->age);
}

int main(int argc,  char **argv)
{
   struct white_human white = {0};
   white.human.age = 23;

   struct black_human black = {0};
   black.human.age = 23;
   _show_human_age(&white);
   _show_human_age(&black);


    return 0;
}
