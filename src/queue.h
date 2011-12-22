/*
 * Author: youngtrips
 * Created Time:  Thu 09 Jun 2011 04:36:17 PM CST
 * File Name: queue.h
 * Description: 
 */

#ifndef __QUEUE_H
#define __QUEUE_H

#include "dlist.h"

typedef struct queue_entry_t {
    void *data;
    dlist_t qnode;
}queue_entry_t;

typedef struct queue_t {
    dlist_t head;
    size_t size;
}queue_t;

queue_t *queue_init();
int queue_push(queue_t *que, void *data);
void *queue_head(queue_t *que);
void *queue_tail(queue_t *que);
void *queue_pop(queue_t *que);
int queue_size(queue_t *que);
int queue_empty(queue_t *que);
void queue_destroy(queue_t *que);

#endif
