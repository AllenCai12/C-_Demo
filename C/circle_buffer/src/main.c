#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "vin_file.h"
//#inc "circle_buffer.h"
#include "vin.h"

int main(int argc, char **argv)
{
    void *buf;
    struct vin_channels *channels;


    printf("start open\n");
    vin_open();

    printf("start vin \n");
    vin_start();
    sleep(1);

    printf("get channels\n");
    channels = vin_get_channels();

    int id =0;
    buf = malloc(channels->channel[id].size);
    memset(buf, 0, channels->channel[id].size);
    int ret;

    do {
        ret = vin_read(id, buf);
        if (ret < 0) {
            break;
        }

        printf("buf:: %s", (char *)buf);

//        sleep(1);

    } while(1);

    sleep(3);
    free(buf);

    return 0;
}

#if 0
int main(void)
{

    int id = 0;
    size_t size = sizeof(char);
    size_t  read_size;
    int ret;

    circular_buf_create(id, size);
    printf("Full: %d, empty: %d\n", circular_buf_full(id), circular_buf_empty(id));

    printf("Adding 9 values\n");
    for(uint8_t i = 0; i < 8; i++)
    {
        ret = circular_buf_put(id, &i);
        if (ret < 0) {
            printf("fail to put data\n");
        }
    }

    printf("Full: %d, empty: %d\n", circular_buf_full(id), circular_buf_empty(id));

    printf("Reading back values: ");
    while(!circular_buf_empty(id))
    {
        uint8_t data;
        circular_buf_get(id, &data);
        printf("%u ", data);
    }
    printf("\n");

    printf("Adding 15 values\n");
    for(uint8_t i = 0; i <  15; i++)
    {
        circular_buf_put(id, &i);
    }

    printf("Full: %d, empty: %d\n", circular_buf_full(id), circular_buf_empty(id));

    printf("Reading back values: ");
    while(!circular_buf_empty(id))
    {
        uint8_t data;
        circular_buf_get(id, &data);
        printf("%u ", data);
    }

    printf("\n");
    circular_buf_clean(id);

    return 0;
}

#endif
