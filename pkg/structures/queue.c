#include "structures.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PAGE_LENGTH 4
#define TOTALMEM 400
#ifndef DATA_H
#define DATA_H
#endif

queue *new_q ()
{
    queue *q = (struct queue *)malloc (sizeof (struct queue));
    q->front = q->rear = NULL;
    q->num = 0;
    return q;
}


void add (queue *q, process *d)
{
    struct process *temp = d;
    q->num++;

    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL)
    {
        q->front = q->rear = temp;
        return;
    }

    // Add the new node at the end of queue and change rear
    q->rear->queueNext = temp;
    q->rear = temp;
}


void add_sorted (queue *q, process *d, bool (*cmp) (process *d, process *t))
{
    struct process *temp = d;
    q->num++;

    struct process *t = q->front;
    struct process *prev = q->front;
    if (cmp != NULL)
    {
        while (t)
        {
            if (cmp (d, t))
            {
                if (t == q->front)
                {
                    q->front = d;
                    d->queueNext = t;
                    return;
                }
                else
                {
                    prev->queueNext = d;
                    d->queueNext = t;
                    return;
                }
            }
            prev = t;
            t = t->queueNext;
        }
    }

    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL)
    {
        q->front = q->rear = temp;
        return;
    }

    // Add the new node at the end of queue and change rear
    q->rear->queueNext = temp;
    q->rear = temp;
}

process *pop (queue *q)
{
    if (q->front == NULL || q->num == 0)
    {
        return NULL;
    }

    process *temp = q->front;
    q->front = q->front->queueNext;
    if (q->front == NULL)
    {
        q->rear = NULL;
    }
    q->num--;
    temp->queueNext = NULL;
    return temp;
}
