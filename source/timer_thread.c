#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include "../include/timer_thread.h"

static unsigned int _timer_thread_node_id = 0;

void tthread_queue_free(node_t *head);
void tthread_queue_push(node_t **head, node_t *node);

void tthread_queue_check(timer_thread_t *thread);

void * timer_thread_run(void *args)
{
    timer_thread_t *thread = (timer_thread_t *)args;

    while (1)
    {
        usleep(thread->interval);

        // printf("timer check=====\n");

        pthread_mutex_lock(&thread->mutex);
        if (thread->stop == 0)
        {
            tthread_queue_check(thread);
            pthread_mutex_unlock(&thread->mutex);
        }
        else
        {
            pthread_mutex_unlock(&thread->mutex);
            break;
        }      
    }

    return NULL;
}

timer_thread_t * tthread_start(unsigned int interval)
{
    timer_thread_t *ret = (timer_thread_t *)malloc(sizeof(timer_thread_t));

    pthread_mutex_init(&ret->mutex, NULL);
    ret->interval = interval;
    ret->stop = 0;
    ret->head = NULL;

    if (pthread_create(&ret->id, NULL, &timer_thread_run, ret) != 0)
    {
        free(ret);
        return NULL;
    }
    
    ret->head = NULL;
    
    return ret;
}

void tthread_end(timer_thread_t **thread)
{
    if (*thread != NULL)
    {
        pthread_mutex_lock(&(*thread)->mutex);
        (*thread)->stop = 1;
        pthread_mutex_unlock(&(*thread)->mutex);

        pthread_join((*thread)->id, NULL);

        tthread_queue_free((*thread)->head);

        pthread_mutex_destroy(&(*thread)->mutex);

        free(*thread);
        *thread = NULL;
    }
}

void tthread_queue_free(node_t *head)
{
    node_t * p = NULL;
    while (head != NULL)
    {
        p = head;
        head = head->next;
        free(p);
    }
}

void tthread_queue_push(node_t **head, node_t *node)
{
    if ((*head) == NULL)
    {
        *head = node;
    }
    else
    {
        node->next = (*head)->next;
        (*head)->next = node;
    }
}

unsigned int tthread_add(timer_thread_t *thread, queue_thread_t *qthread, void *data, unsigned int delay, unsigned int interval)
{
    if (thread != NULL)
    {
        node_t * p = (node_t *)malloc(sizeof(node_t));
        p->id = ++ _timer_thread_node_id;
        p->delay = delay;
        p->interval = interval;
        p->thread = qthread;
        p->data = data;
        p->next = NULL;

        pthread_mutex_lock(&thread->mutex);
        tthread_queue_push(&thread->head, p);
        pthread_mutex_unlock(&thread->mutex);

        return _timer_thread_node_id;
    }
    return 0;   
}

void tthread_remove(timer_thread_t *thread, unsigned int id)
{
    if (thread != NULL)
    {
        pthread_mutex_lock(&thread->mutex);

        if (thread->head == NULL)
        {
            pthread_mutex_unlock(&thread->mutex);
            return;       
        }
    
        if (thread->head->id == id)
        {
            node_t *p = thread->head;
            thread->head = thread->head->next;
            free(p);

            pthread_mutex_unlock(&thread->mutex);
            return;
        }
        
        node_t *p1 = thread->head;
        node_t *p2 = p1->next;
        while (p2 != NULL)
        {
            if (p2->id == id)
            {
                p1->next = p2->next;
                free(p2);
                break;
            }
            p1 = p2;
            p2 = p1->next;
        }

        pthread_mutex_unlock(&thread->mutex); 
    }   
}

void tthread_queue_check(timer_thread_t *thread)
{
    int keep_head = 0;

    node_t *p = thread->head;
    while (p != NULL)
    {
        p->delay -= thread->interval;
        if (p->delay <= 0)
        {
            qthread_push(p->thread, p->data);
            if (p->interval == 0)
            {
                node_t * t = p;
                p = p->next;
                free(t);

                if (keep_head == 0)
                {
                    thread->head = p;
                }
            }
            else
            {
                p->delay = p->interval;
            }
        }
        else
        {
            keep_head = 1;
            p = p->next;
        }
    }
}