//
// Created by alan on 18-6-21.
//
#include <stdio.h>
#include <stdlib.h>
#include <log.h>
#include <type.h>
#include <string.h>
#include "vin_file.h"


#define HD_WIDTH                1920
#define HD_HEIGHT               1088

//#define HD_WIDTH                1
//#define HD_HEIGHT               256

#define MAX_FILE_COUNT          128

#define GET_ARRAY_LEN(array, len) do { \
                                  len = sizeof(array) / sizeof(array[0]);\
                                  }while(0)


static char files[][128] = {
        "1080P.h264"
};

//        "test_b.h264",
//        "test_c.h264",
//        "test_d.h264",
static FILE *   s_fds[MAX_FILE_COUNT]; //已经打开的文件描述符
static int      s_file_count; //需要打开的文件数量
//static void *   s_frame_buf[MAX_FILE_COUNT]; //从文件获取的一帧数据的暂存区
static size_t   s_buf_size;

int vin_file_open()
{
    int ret = 0;
    int i;
    s_buf_size = (HD_WIDTH * HD_HEIGHT * 3) / 2;

    GET_ARRAY_LEN(files, s_file_count);
    for (i=0; i<s_file_count; i++) {
        s_fds[i] = fopen(files[i], "r");
        if (s_fds == NULL) {
            log_handle_error("fopen");
            perror("reason: ");
            ret = -1;
            break;
        }

#if 0
        s_frame_buf[i] = calloc(1, s_buf_size);
        if (s_frame_buf[i] == NULL) {
            log_handle_error("calloc");
            perror("reason: ");
            ret = -1;
            break;
        }
#endif

    }

    return ret;
}

int vin_file_get_channels(struct vin_channels *channels)
{
    int i;
    channels->count = s_file_count;

    for (i=0; i<channels->count; i++) {
        channels->channel[i].attr.format = H264;
        channels->channel[i].attr.height = HD_WIDTH;
        channels->channel[i].attr.width = HD_HEIGHT;
        channels->channel[i].attr.source = DISK;
        channels->channel[i].size = (HD_WIDTH * HD_HEIGHT * 3)/2 ;
    }

    return 0;
}

size_t vin_file_read(int id, void *buf)
{
    size_t read_len;
    static size_t byte_used[MAX_FILE_COUNT] = {0} ;

    fseek(s_fds[id], byte_used[id], SEEK_SET);
    read_len = fread(buf, 1, s_buf_size, s_fds[id]);
    if (read_len == 0) {
        byte_used[id] = 0;
        fseek(s_fds[id], 0, SEEK_SET);
        read_len = fread(buf, 1, s_buf_size, s_fds[id]);
    }
    byte_used[id] += read_len;

    return read_len;
}

void vin_file_close()
{
    int i;
    for (i=0; i<s_file_count; i++) {
        fclose(s_fds[i]);
    }
}
