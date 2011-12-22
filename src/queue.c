/*
 * Author: youngtrips
 * Created Time:  Thu 09 Jun 2011 04:36:23 PM CST
 * File Name: queue.c
 * Description: 
 */

#include <stdlib.h>

#include "queue.h"


#define INIT_CHUNK_SIZE 16
typedef struct queue_chunk_t {
    size_t size;
    size_t head;
    size_t tail;
    dlist_t node;
    void *chunk[1];
} queue_chunk_t;

struct queue_t {
    size_t chunk_num;
    dlist_t chunk_list;
    queue_chunk_t *rpos;
    queue_chunk_t *wpos;
};

queue_t *queue_init()
{
}

int queue_push(queue_t *que, void *data)
{
}

void *queue_head(queue_t *que)
{
}

void *queue_tail(queue_t *que)
{
}

void *queue_pop(queue_t *que)
{
}

size_t queue_size(queue_t *que)
{
}

int queue_empty(queue_t *que)
{
}

void queue_destroy(queue_t *que)
{
}


