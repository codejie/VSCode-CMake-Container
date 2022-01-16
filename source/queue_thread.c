#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/queue_thread.h"

void * queue_thread_run(void* args)
{    
    printf("thread run begin.\n");

    queue_thread_t * thread = (queue_thread_t *)args;

    while (1)
    {
        pthread_mutex_lock(&thread->mutex);
        if (thread->stop == 0)
        {
            pthread_cond_wait(&thread->cnd, &thread->mutex);
            void * data = queue_pop(thread->que);
            pthread_mutex_unlock(&thread->mutex);
            (*thread->func)(data);
        }
        else
        {
            pthread_mutex_unlock(&thread->mutex);
            break;
        }        
    }

    printf("thread run end.\n");
    // sleep(3);

    return NULL;
}

queue_thread_t * qthread_start(func_t func)
{
    queue_thread_t *ret = (queue_thread_t *)malloc(sizeof(queue_thread_t));

    pthread_mutex_init(&ret->mutex, NULL);
    pthread_cond_init(&ret->cnd, NULL);
    ret->stop = 0;
    ret->func = func;
    ret->que = queue_create(128);

    if (pthread_create(&ret->id, NULL, &queue_thread_run, ret) != 0)
    {
        free(ret);
        printf("thread create fail.\n");
        return NULL;
    }

    printf("thread create ok.\n");

    return ret;
}

void qthread_end(queue_thread_t **thread)
{
    if (*thread != NULL)
    {
        pthread_mutex_lock(&(*thread)->mutex);
        (*thread)->stop = 1;
        pthread_mutex_unlock(&(*thread)->mutex);

        pthread_join((*thread)->id, NULL);

        printf("thread be ended.\n");

        queue_free(&(*thread)->que);
        pthread_mutex_destroy(&(*thread)->mutex);
        pthread_cond_destroy(&(*thread)->cnd);

        free(*thread);
        *thread = NULL;
    }
}

void qthread_push(queue_thread_t *thread, void *data)
{
    if (thread != NULL && thread->id != 0)
    {
        pthread_mutex_lock(&thread->mutex);
        queue_push(thread->que, data);
        pthread_cond_signal(&thread->cnd);
        pthread_mutex_unlock(&thread->mutex);
    } 
}