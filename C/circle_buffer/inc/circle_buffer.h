#ifndef C_CIRCLE_BUF_H
#define C_CIRCLE_BUF_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct circular_buf{
    void * buffer;
    size_t head;
    size_t tail;
    size_t frame_size; //of the frame
};

/**
 * Important Usage Note: This library reserves one spare byte for queue-full detection
 * Otherwise, corner cases and detecting difference between full/empty is hard.
 * You are not seeing an accidental off-by-one.
 */
int circular_buf_create(int id, size_t frame_size);

int circular_buf_clean(int id);

int circular_buf_reset(int id);
/// Put Version 1 allows you to keep adding entries if the buffer is full
int circular_buf_put(int id, void *data);
/// Put Version 2 returns an error if the buffer is full
int circular_buf_put2(int id, uint8_t data);
//TODO: int circular_buf_put_range(circular_buf_t cbuf, uint8_t * data, size_t len);
int circular_buf_get(int id, void *buf);
//TODO: int circular_buf_get_range(circular_buf_t cbuf, uint8_t *data, size_t len);
bool circular_buf_empty(int id);
bool circular_buf_full(int id);

#endif
