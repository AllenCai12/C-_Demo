/*
 * File: seq.c
 * Author: Terry Lei <freedom@ema-tech.com>
 * Brief: Imlementation of types defined in seq.h
 */
#include "seq.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct seq {
    void **data;
    int head;
    int length;
    int size;
};

struct seq *seq_new(int hint) {
    assert(hint >= 0);
    int size = hint==0? 16 : hint; 
    struct seq *seq = malloc(sizeof(*seq));
    seq->data = malloc(sizeof(void *)*size);
    memset(seq->data, 0, sizeof(void *)*size);
    seq->head = 0;
    seq->length = 0;
    seq->size = size;
    return seq;
}

void seq_release(struct seq *seq) {
    free(seq->data);    
    free(seq);
}

int seq_length(struct seq *seq) {
    return seq->length;
}

void seq_clear(struct seq *seq) {
    seq->head = 0;
    seq->length = 0;    
}
    
static void __expand(struct seq *seq) {
    assert(seq->length == seq->size);
    int new_size = seq->size * 2;
    int n = seq->size - seq->head; 
    seq->data = realloc(seq->data, sizeof(void *)*new_size);
    seq->size = new_size;
    // fix head
    void **old = seq->data + seq->head;
    void **new = seq->data + new_size - n;
    memmove(new, old, sizeof(void *)*n);
    seq->head = new_size - n; 
}

static inline int __mod(int a, int b) {
    assert(b > 0);
    if (a >= 0) {
        return a % b;
    }
    else {
        if (-13 % 5 == 2) 
            return a % b;
        else 
            return a % b + b;
    }
}

static inline int __seq_offset(struct seq *seq, int index) {
    return __mod(seq->head + index, seq->size);
}

void seq_push_back(struct seq *seq, void *elem) {
    if (seq->length == seq->size)    
        __expand(seq);
    seq->data[__seq_offset(seq, seq->length)] = elem;
    seq->length++;
}

void seq_push_front(struct seq *seq, void *elem) {
    if (seq->length == seq->size)
        __expand(seq);
    seq->head = __mod(seq->head-1, seq->size);
    seq->data[seq->head] = elem;
    seq->length++;
}

void * seq_pop_front(struct seq *seq) {
    assert(seq->length > 0);
    void *elem = seq->data[seq->head]; 
    seq->head = __mod(seq->head+1, seq->size);
    seq->length--;
    return elem;
}

void * seq_pop_back(struct seq *seq) {
    assert(seq->length > 0);
    void *elem = seq->data[__seq_offset(seq, seq->length-1)];
    seq->length--;
    return elem;
}

static inline bool __is_space_continuous(struct seq *seq) {
    return seq->size - seq->head >= seq->length;
}

void seq_update(seqi iter) {
    if (iter->begin == NULL)
        return;
    if (iter->offset >= 0 && iter->offset < iter->length)
        return;
    struct seq *seq = iter->seq;
    assert(seq);
    assert(iter->begin == seq->data + seq->head
        || iter->begin == seq->data);
    if (__is_space_continuous(seq)) {
        iter->begin = NULL; /* out of range  */
        return;
    }
    /* space is not continuous */
    if (iter->begin == seq->data + seq->head) {
        /* the block that starts with head:
         *  |####-----------#######|
         *                  |
         *                 Head (iter->begin)
         */
        if (iter->offset < 0) {
            iter->begin = NULL; 
        }
        else {
            iter->begin = seq->data;    
            iter->offset = 0;
            iter->length = seq->length - (seq->size - seq->head);
        }    
    }
    else {
        /* the block that starts with data buffer:
         *  |####-----------#######|
         *   |              |
         *  (iter->begin)  Head
         */
        if (iter->offset < 0) {
            iter->begin = seq->data + seq->head;
            iter->offset = seq->size - seq->head - 1;
            iter->length = seq->size - seq->head;
        } 
        else {
            iter->begin = NULL;
        }
    }
    assert(iter->begin == NULL
        || iter->begin == seq->data + seq->head
        || iter->begin == seq->data);
}

void seq_head(struct seq *seq, seqi iter) {
    iter->seq = seq;
    if (seq->length == 0) {
        iter->begin = NULL;
        return;
    }
    iter->begin = seq->data + seq->head;
    iter->offset = 0;
    if (__is_space_continuous(seq))
        iter->length = seq->length;
    else 
        iter->length = seq->size - seq->head;
}

void seq_tail(struct seq *seq, seqi iter) {
    iter->seq = seq;
    if (seq->length == 0) {
        iter->begin = NULL;
        return;
    }
    if (__is_space_continuous(seq)) {
        iter->begin = seq->data + seq->head;
        iter->offset = seq->length-1;
        iter->length = seq->length;
    }
    else {
        iter->begin = seq->data;
        iter->offset = seq->length - (seq->size - seq->head) - 1;
        iter->length = iter->offset + 1;
    }
}

