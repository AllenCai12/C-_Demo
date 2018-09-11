//
// Created by alan on 18-6-21.
//

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <log.h>
#include "circle_buffer.h"
#include "vin.h"
#include "vin_file.h"

BOOL s_is_file_exit;
struct vin_channels s_sum_channels;
static pthread_t s_pthread[MAX_CHANNEL_COUNT];
BOOL    s_stop_flag[MAX_CHANNEL_COUNT];


void vin_handle(void *ptr)
{
    int id = (int)ptr;
    int ret;
    size_t read_len;
    SOURCE source;
    void *buf;

    source = s_sum_channels.channel[id].attr.source;
    buf  = malloc(s_sum_channels.channel[id].size);

    printf("start disk run: %d\n", source);
    switch (source) {
        case DISK: {
            printf("start create buf %d\n", source);
            circular_buf_create(id, s_sum_channels.channel[id].size);
            while(!s_stop_flag[id]) {
                if (circular_buf_full(id)) {
                    usleep(10);
                    continue;
                }
                printf("start read file: %lu\n", s_sum_channels.channel[id].size);
                read_len = vin_file_read(id, buf);
                if (read_len == 0) {
                    s_stop_flag[id] = FALSE;
                    break;
                }
                printf("start put file: size: %lu\n", s_sum_channels.channel[id].size);
                ret = circular_buf_put(id, buf);
                if (ret < 0) {
                    break;
                }
            }
            break;
        }

        default:
            break;
    }

    free(buf);
}


int  vin_open()
{
    int ret = 0;
    int i;

    /*init stop flag*/
    for (i=0; i<MAX_CHANNEL_COUNT; i++){
        s_stop_flag[i] = FALSE;
    }

    ret = vin_file_open();
    if (ret < 0) {
        s_is_file_exit = FALSE;
    }
    else{
        s_is_file_exit = TRUE;
    }

    if (s_is_file_exit) {
        vin_file_get_channels(&s_sum_channels);
    }

    return  ret;
}

int vin_start()
{
    int ret = -1;
    int i;

    for (i=0; i<s_sum_channels.count; i++) {

        printf("start pthread \n");
        ret = pthread_create(&s_pthread[i], NULL, (void *) &vin_handle, (void*)i);
        if (ret != 0) {
            ret = -1;
            log_handle_error("create pthread");
            perror("reason: ");
            break;
        }
        pthread_detach(s_pthread[i]);
        ret = 0;
    }

    return ret;
}


struct vin_channels * vin_get_channels()
{
    return &s_sum_channels;
}

int  vin_channels_add(struct vin_channels *dest_channels, struct vin_channels *src_channels)
{
    int ret = 0;
    int count;
    int i;

    count = dest_channels->count + src_channels->count;

    for (i=0; i<src_channels->count; i++) {
        dest_channels->channel[dest_channels->count] = src_channels->channel[i];
    }

    dest_channels->count = count;

    return  ret;
}

int vin_read(int id, void *buf)
{
    int ret = 0;
    if (s_stop_flag[id] == TRUE) {
        printf("stop \n");
        return -1;
    }

    if (!circular_buf_empty(id)) {
        ret = circular_buf_get(id, buf);
        if (ret < 0) {
            log_handle_error("get data from buf");
        }
    }
    else {
        printf("buf is empty\n");
    }


    return  ret;
}

int vin_stop(int id)
{
    s_stop_flag[id] = FALSE;
}

int  vin_close()
{
    int ret = 0;

    if (s_is_file_exit) {
        vin_file_close();
    }
    return  ret;
}
