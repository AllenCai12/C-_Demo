#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

int main(int argc, char **argv)
{
    struct timeval start, end;
    gettimeofday( &start, NULL );

    usleep(1000);

    gettimeofday( &end, NULL );
    __time_t time_use = 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec -start.tv_usec;
    printf("recv time: %ld ms\n", time_use/1000);


    return 0;
}
