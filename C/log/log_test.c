#include <stdio.h>

#ifdef DEBUG
#define debug_str(s)    do{\
                        printf("function: %s, line: %d, msg: %s\n",\
                                __FUNCTION__, __LINE__, s); \
                    }while(0)

#define debug_int(i)    do{\
                        printf("function: %s, line: %d, msg: %ld\n",\
                                __FUNCTION__, __LINE__, (long int)i); \
                    }while(0)
#else

#define debug_int(i)
#define debug_str(s)

#endif


int main(int argc, char **argv)
{
    int i = 20;

    debug_str("hello world");
    debug_int(i);
}