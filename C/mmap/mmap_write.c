#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>


void mmap_write()
{
    int i = 0;
    char *pMap = NULL;
    int iPagesize;
    int fd;
    unsigned long  ulStart;
    struct timeval struStart;
    struct timeval struStop;

    char strData[STR_LEN] = {0};
    char strLeftData[2048] = {0};   
    unsigned short strSize = sizeof(strLeftData);

    gettimeofday(&struStart, NULL);
    ulStart = clock();

#ifndef WRITE_FILE
    pMap = (char *)calloc(1, (STR_LEN-1)/4);
#endif

    fd = open("data.txt",O_RDWR|O_CREAT,00600);
    ftruncate(fd,  COUNT*(STR_LEN-1));

    for (i = 0; i < 4*COUNT; i++)
    {
#ifdef WRITE_FILE
        pMap = (char *)mmap(0,(STR_LEN-1)/4, PROT_WRITE, MAP_SHARED, fd, i*(STR_LEN-1)/4); // 每次打开1M
        //ftruncate(fd,  i*(STR_LEN-1)/4 + (STR_LEN-1)/4 );
#endif
        setRecordData(strData, (STR_LEN-1)/4 +1 , strLeftData, strSize);
        memcpy(pMap , strData, (STR_LEN-1)/4);
#ifdef WRITE_FILE
        munmap(pMap, (STR_LEN-1)/4);
#endif
    }    
    close(fd);
#ifndef WRITE_FILE
    free(pMap);
#endif

    printf("mmap_write5_2:   %.3lf(clock time) (clock time), mmap: 1M buffer\n", double(clock()-ulStart)/CLOCKS_PER_SEC);
    gettimeofday(&struStop, NULL);
    printf("all time diff:   %.3lf\n\n", diffTime(struStop, struStart));
}

int main(int argc, char **argv)
{


    return 0;
}
