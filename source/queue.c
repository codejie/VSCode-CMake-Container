#include <stdio.h>
#include <stdlib.h>

#include "../include/queue.h"

// static _node_t *_head = NULL;
// static _node_t *_tail = NULL;


queue * queue_create(unsigned int max)
{
    queue * ret = (queue *)malloc(sizeof(queue)); 
    ret->_max = max;
    ret->_tail = NULL;

    return ret;
}

void queue_push(queue* que, void *p)
{
    if (que != NULL)
    {
        _node_t* n = (_node_t *)malloc(sizeof(_node_t));
        n->value = p;
        n->pre = NULL;

        if (que->_tail == NULL)
        {
            que->_tail = n;
        }
        else
        {
            _node_t *p = que->_tail;
            unsigned int count = 0;
            while (p->pre != NULL)
            {
                if (++ count >= que->_max)
                {
                    printf("ERROR - out of queue.");
                    return;
                }
                p = p->pre;
            }

            p->pre = n;
        }
    }
}

void *queue_pop(queue *que)
{
    if (que != NULL && que->_tail != NULL)
    {
        _node_t *p = que->_tail;
        que->_tail = p->pre;

        void *ret = p->value;

        free(p);    

        return ret;
    }
    return NULL;
}

void queue_clear(queue *que)
{
    while(que != NULL && que->_tail != NULL)
    {
        _node_t *p = que->_tail->pre;
        if (que->_tail->value != NULL)
        {
            free(que->_tail->value);
            que->_tail->value = NULL;
        }
        free(que->_tail);
        que->_tail = p;
    }
}

int queue_is_empty(queue *que) 
{
    if (que != NULL && que->_tail == NULL)
    {
        return 1;
    }

    return 0;
}

int queue_count(queue *que)
{
    if (que != NULL)
    {
        int ret = 0;
        _node_t * p = que->_tail;
        while(p != NULL)
        {
            ++ ret;
            p = p->pre;
        }

        return ret;
    }
    return 0;
}

void queue_free(queue **que)
{
    if (*que != NULL)
    {
        queue_clear(*que);
        free(*que);
        *que = NULL;
    }
}