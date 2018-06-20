#include <time.h>
#include <string.h>
#include <stdio.h>


const char *get_time() 
{
    static char buf[256] = {0};
    time_t raw_time; 
    struct tm *time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%d%02d%02d%02d%02d",  time_info->tm_year+1900, \
                                        time_info->tm_mon,  time_info->tm_mday,\
                                        time_info->tm_hour, time_info->tm_min);

    return buf;
}

int main(int argc, char **argv)
{

    printf("time :%s\n", get_time());

    return 0;
}

