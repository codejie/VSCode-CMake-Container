#ifndef __Z_QUEUE_H__
#define __Z_QUEUE_H__

// #define _value_t void

typedef struct __node_t _node_t;

struct __node_t {
    void *value;
    _node_t *pre;
};

typedef struct {
    _node_t *_tail;
    unsigned int _max;
} queue;

queue * queue_create(unsigned int max);
void queue_push(queue *que, void *p);
void * queue_pop(queue *que);
void queue_clear(queue *que);
int queue_is_empty(queue *que);
int queue_count(queue *que);
void queue_free(queue **que);

#endif