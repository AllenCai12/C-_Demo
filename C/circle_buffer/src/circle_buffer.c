#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circle_buffer.h"

#define MAX_SIZE    128
#define FRAME_COUNT  10 //缓存的帧数
static struct circular_buf s_buf[MAX_SIZE];


int circular_buf_create(int id, size_t frame_size)
{
   int ret = 0;
   s_buf[id].frame_size = frame_size;
   s_buf[id].buffer = calloc(FRAME_COUNT, frame_size);
   if (s_buf[id].buffer == NULL) {
       perror("malloc");
       ret = -1;
       goto error;
   }

    s_buf[id].head = 0;
    s_buf[id].tail = 0;
   error:
    return ret;
}

int circular_buf_clean(int id)
{
    int ret = 0;
    if (s_buf[id].buffer) {
        free(s_buf[id].buffer);
        s_buf[id].buffer = NULL;
    }
    return ret;
}


int circular_buf_reset(int id)
{
    int r = -1;

    if(s_buf[id].buffer) {
        s_buf[id].head = 0;
        s_buf[id].tail = 0;
        r = 0;
    }

    return r;
}

int circular_buf_put(int id, void *data)
{
    int r = -1;

    if(s_buf[id].buffer) {
        memcpy(s_buf[id].buffer + s_buf[id].head, data, s_buf[id].frame_size);
        s_buf[id].head = (s_buf[id].head + s_buf[id].frame_size) % (s_buf[id].frame_size * FRAME_COUNT);

        if(s_buf[id].head == s_buf[id].tail)
        {
            s_buf[id].tail = (s_buf[id].tail + s_buf[id].frame_size) % (s_buf[id].frame_size * FRAME_COUNT);
        }

        r = 0;
    }

    return r;
}

#if 0
int circular_buf_put2(int id, uint8_t data)
{
    int r = -1;

    if(s_buf[id].buffer && !circular_buf_full(id)) {
        s_buf[id].buffer[s_buf[id].head] = data;
        s_buf[id].head = (s_buf[id].head + 1) % s_buf[id].frame_size;

        if(s_buf[id].head == s_buf[id].tail) {
            s_buf[id].tail = (s_buf[id].tail + 1) % s_buf[id].frame_size;
        }

        r = 0;
    }

    return r;
}
#endif

int circular_buf_get(int id, void *buf)
{
    int r = -1;

    if (!buf){
        printf("buf is empty\n");
    }
    if (!s_buf[id].buffer) {
        printf("s_buf is empty\n");
    }

    if(s_buf[id].buffer && buf && !circular_buf_empty(id)) {
        //*size = s_buf[id].frame_size;
        printf("start get data\n");
        memcpy(buf, s_buf[id].buffer+s_buf[id].tail, s_buf[id].frame_size);
        s_buf[id].tail = (s_buf[id].tail + s_buf[id].frame_size) % (s_buf[id].frame_size * FRAME_COUNT);
        r = 0;
    }

    return r;
}

bool circular_buf_empty(int id)
{
    // We define empty as head == tail
    return (s_buf[id].buffer && (s_buf[id].head == s_buf[id].tail));
}

bool circular_buf_full(int id)
{
    // We determine "full" case by head being one position behind the tail
    // Note that this means we are wasting one space in the buffer!
    // Instead, you could have an "empty" flag and determine buffer full that way
    return (s_buf[id].buffer && ((s_buf[id].head + s_buf[id].frame_size) % (s_buf[id].frame_size * FRAME_COUNT) ) == s_buf[id].tail);
}
