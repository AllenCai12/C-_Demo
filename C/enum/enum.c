#include <stdio.h>
#include <stdlib.h>


enum STORAGE
{
    USB, SD, EMMC
};

enum STORAGE get_storage(int dev)
{
    switch (dev){

    case USB:
        return USB;
        break;
    case SD:
        return SD;
        break;
    case EMMC:
        return EMMC;
        break;
    default :
        printf("wrong device\n");
        exit(1);
        break;
    }
}

int main(int agrc, char **argv)
{
    
    printf("USB=%d\n", USB);
    printf("SD=%d\n", SD);
    printf("EMMC=%d\n", EMMC);

    printf("device=%d\n", get_storage(atoi(argv[1])));

    return 0;
}
