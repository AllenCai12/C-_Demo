#include <stdio.h>

struct area {
    int x;
    int y;
    int w;
    int h;
};

int main(int argc, char **argv)
{
    struct area area_a= {1, 2, 3, 4};
    struct area area_b= {1, 3, 3, 4};
    struct area area_c= {1, 2, 3, 4};

    if (area_a == area_b) {
        printf("a and b is equal");
    }

    if (area_a == area_c) {
        printf("a and c is equal");
    }

    return 0;
}
