#ifndef __Z_QUEUE_THREAD_H__
#define __Z_QUEUE_THREAD_H__

#include <pthread.h>

#include "queue.h"

typedef void (*func_t)(void *);

typedef struct {
    pthread_t id;
    queue* que;

    short stop;
    pthread_mutex_t mutex;
    pthread_cond_t cnd;

    func_t func;
} queue_thread_t;


queue_thread_t * qthread_start(func_t func);
void qthread_end(queue_thread_t **thread);
void qthread_push(queue_thread_t *thread, void *data);


#endif