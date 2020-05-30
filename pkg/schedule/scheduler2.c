
#include "scheduler.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define MEMLEN 4
#define TOTALMEM 400
#define MEMALGO 1
int left (datat *head)
{
    if (head == NULL)
    {
        return 0;
    }
    int count = 0;
    datat *data = head;
    while (data)
    {
        count += data->remaining != 0;
        data = data->llNext;
    }
    return count;
}

int next (datat *head, enum scheduler type, int quantum)
{
    int time = 0;
    int remaining = left (head);
    do
    {
        time = next_helper (head, type, quantum, time);
        remaining = left (head);
    } while (remaining);
    return 0;
}

int next_helper (datat *head, enum scheduler type, int quantum, int time)
{
    queue *q = NewQueue ();
    int remaining = left (head);
    if (head == NULL)
    {
        return time;
    }
    datat *next = NULL;
    datat *data = head;
    mem memory;
    memory.len = 0;
    memory.memory = (page **)calloc (TOTALMEM / MEMLEN, sizeof (page *));
    assert (memory.memory);
    while (remaining != 0)
    {
        while (data)
        {
            if (time < data->arrival)
            {
                break;
            }
            if (time >= data->arrival)
            {
                addToQueue (q, data);
            }
            data = data->llNext;
        }
        if (next != NULL && next->remaining != 0)
        {
            addToQueue (q, next);
        }
        next = getFromQueue (q);

        time = assign_memory (memory, q, next, quantum, time);
        time = apply_quantum (head, next, quantum, time);


        remaining = left (head);
    }
    return time;
}

int memoryallocate (mem memory, queue *q, page *p, int time)
{
    // for (q->rear->memsize)
    datat *oldest = q->front->queueNext; // the front of the queue is this process, so the one following is the last executed.
    int pageid = -1;
    // TODO: assign memory,
    for (int i = 0; i < memory.len; i++)
    {
        if (memory.memory[i] == NULL)
        {
            memory.memory[i] = p;
            if (pageid == -1)
            {
                p->id = i;
            }
        }
    }

    bool cont = true;
    do
    {

        for (int i = 0; i < oldest->memunits; i++)
        {
            if (MEMALGO == 0)
            {
                if (oldest->memory[i]->allocated)
                {
                    cont = false;
                    memory.memory[oldest->memory[i]->id] = NULL;
                    oldest->memory[i]->allocated = false;
                }
            }
            else
            {
                if (oldest->memory[i]->allocated)
                {
                    oldest->memory[i]->allocated = false;
                    p->id = oldest->memory[i]->id;
                    memory.memory[p->id] = p;
                    return time;
                }
            }
        }
        oldest = oldest->queueNext;
    } while (cont && oldest != NULL);
    return memoryallocate (memory, q, p, time);
}

// assign_memory assigns memory to a process
int assign_memory (mem memory, queue *q, datat *next, int quantum, int time)
{
    for (int i = 0; i < next->memsize; i++)
    {
        page *p = next->memory[i];
        if (p->allocated == false)
        {
            time = memoryallocate (memory, q, p, time);
        }
    }
    return time;
}

int apply_quantum (datat *head, datat *next, int quantum, int time)
{
    if (next->remaining == 0)
    {
        return time;
    }
    printf ("%d, RUNNING, id=%d, remaining-time=%d\n", time, next->procid, next->remaining);
    if (quantum == -1)
    {
        time += next->remaining;
        next->remaining = 0;
    }
    else if (quantum < next->remaining)
    {
        time += quantum;
        next->remaining -= quantum;
        return time;
    }
    else if (quantum == next->remaining)
    {
        time += quantum;
        next->remaining = 0;
    }
    else
    {
        time += quantum - next->remaining;
        next->remaining = 0;
    }
    printf ("%d, FINISHED, id=%d, proc-remaining=%d\n", time, next->procid, left (head));
    return time;
}
