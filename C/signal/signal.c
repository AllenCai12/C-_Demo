#include <stdio.h>
#include <signal.h>


int stop = 0;
void handl(int sig) 
{
    printf("sig = %d\n", sig);
    
}

int main(int argc, char **argv)
{

    signal(SIGINT, handl); 
    do {
        scanf("%d", &stop);

    } while(!stop);

    return 0;
}
