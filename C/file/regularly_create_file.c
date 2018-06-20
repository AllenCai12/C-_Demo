#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>

const long unsigned get_time(char *buf)
{
    long unsigned temp;

    time_t raw_time;
    struct tm *time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%d%02d%02d%02d%02d%02d_hello_123.wav",  time_info->tm_year+1900, \
                                        time_info->tm_mon,  time_info->tm_mday,\
                                        time_info->tm_hour, time_info->tm_min,\
                                        time_info->tm_sec);
    printf("buf : %s\n", buf);
    return strtoul(buf, NULL, 10);
}

int create_file(char *file_name)
{
    FILE *fp;
    int ret;

    assert(file_name);
    do {
        fp = fopen(file_name, "w+");
        if (fp == NULL) {
            perror("open file");
            break;
        }

    } while(0);

    if (fp != NULL) {
        fclose(fp);
    }

}



int main(int argc, char *argv)
{
    char buf[256];
    char file_name[256];

    mkdir("./tmp", S_IRWXU);
    int i = 18;

    while (i--){
        printf("time: %lu\n", get_time(buf));
        sprintf(file_name, "./tmp/%s", buf);
        create_file(file_name);
        memset(file_name, 0, sizeof(file_name));
        sleep(1);
    }

    return 0;
}
