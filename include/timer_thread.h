#ifndef __Z_TIMER_THREAD_H__
#define __Z_TIMER_THREAD_H__

#include <pthread.h>
#include "queue_thread.h"

typedef struct _node_t node_t;

struct _node_t {
    unsigned int id;
    // unsigned short type;
    unsigned int delay;
    unsigned int interval;

    queue_thread_t* thread;
    void *data;

    node_t* next;
};

typedef struct {
    pthread_t id;
    short stop;
    pthread_mutex_t mutex;
    // pthread_cond_t cnd;

    unsigned short interval;
    node_t* head;
} timer_thread_t;


timer_thread_t *tthread_start(unsigned int interval);
void tthread_end(timer_thread_t **thread);
unsigned int tthread_add(timer_thread_t *thread, queue_thread_t *qthread, void* data, unsigned int delay, unsigned int interval);
void tthread_remove(timer_thread_t *thread, unsigned int id);

#endif


