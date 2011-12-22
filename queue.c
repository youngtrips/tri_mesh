/*
 * Author: youngtrips
 * Created Time:  Thu 09 Jun 2011 04:36:23 PM CST
 * File Name: queue.c
 * Description: 
 */

#include <stdlib.h>

#include "queue.h"

queue_t *queue_init() 
{
    queue_t *que;

    que = (queue_t*)malloc(sizeof(queue_t));
    if(!que)
        return NULL;
    init_dlist_node(&(que->head));
    que->size = 0;
    return que;
}

int queue_push(queue_t *que, void *data) {
    queue_entry_t *entry;
    entry = (queue_entry_t*)malloc(sizeof(queue_entry_t));
    if(!entry)
        return -1;
    entry->data = data;
    init_dlist_node(&(entry->qnode));
    dlist_add_tail(&(entry->qnode), &(que->head));
    que->size++;
    return 0;
}

void *queue_head(queue_t *que) {
    queue_entry_t *entry;
    dlist_t *p;

    if(que->size == 0)
        return NULL;
    p = que->head.next;
    entry = (queue_entry_t*)dlist_entry(p, queue_entry_t, qnode);
    return entry->data;
}

void *queue_tail(queue_t *que) {
    queue_entry_t *entry;
    dlist_t *p;

    if(que->size == 0)
        return NULL;
    p = que->head.prev;
    entry = (queue_entry_t*)dlist_entry(p, queue_entry_t, qnode);
    return entry->data;
}

void *queue_pop(queue_t *que) {
    void *res;

    queue_entry_t *entry;
    dlist_t *p;

    res = queue_head(que);
    if (res == NULL) {
        return NULL;
    }

    p = que->head.next;
    dlist_del(p);
    entry = (queue_entry_t*)dlist_entry(p, queue_entry_t, qnode);
    free(entry);
    que->size--;
    return res;
}

int queue_size(queue_t *que) {
    return que->size;
}

int queue_empty(queue_t *que) {
    return que->size == 0;
}

void queue_destroy(queue_t *que) {
    queue_entry_t *entry;
    dlist_t *p;
    dlist_t *q;
    if(!que) return;
    dlist_foreach_safe(p, q, &(que->head)) {
        entry = (queue_entry_t*)dlist_entry(p, queue_entry_t, qnode);
        free(entry);
    }
    free(que);
}


