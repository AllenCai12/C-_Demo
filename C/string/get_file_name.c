#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    char *file_name;
    if( argc != 2) {
        printf("Usage: [path]\n");
        return 1;
    }
    
    file_name = strrchr(argv[1], '/') + 1;
    printf("file_name: %s\n", file_name);


    return 0;
}

