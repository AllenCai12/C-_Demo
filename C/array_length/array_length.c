#include <stdio.h>

#define GET_ARRAY_LEN(array, len) do { \
                                  len = sizeof(array) / sizeof(array[0]);\
                                  }while(0)

static char files[][256] = {
                        "hello world",
                        "come on",
                        "I am comming",
                        "ddsdf",
                        "I am OK"
                        };

int main(int argc, char **argv)
{

    long int length;
    int i;
//    length = sizeof(files) / sizeof(files[0]); 
    GET_ARRAY_LEN(files, length);
    printf("files length: %ld\n", length);

    for (i=0; i < length; i++) {
        printf("%s\n", files[i]);
    }

    return 0;
}
