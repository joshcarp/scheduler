
#include "scheduler.h"

int left (process *head)
{
    if (head == NULL)
    {
        return 0;
    }
    int count = 0;
    process *data = head;
    while (data)
    {
        count += data->remaining != 0;
        data = data->llNext;
    }
    return count;
}

int next (process *head, enum scheduler type, int quantum)
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


int next_helper (process *head, enum scheduler type, int quantum, int time)
{
    queue *q = NewQueue ();
    int remaining = left (head);
    if (head == NULL)
    {
        return time;
    }
    process *next = NULL;
    process *data = head;
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

        time = assign_memory(memory, q, next, quantum, time);
        time = apply_quantum (head, next, quantum, time);


        remaining = left (head);
    }
    return time;
}

int memoryallocate (mem memory, queue *q, page *p, int time)
{
    // for (q->rear->memsize)
    process *oldest = q->rear;
    int remainingsize = p->size;
    int pageid = -1;
    // TODO: assign memory, 
    while (remainingsize > 0)
    {
        for (int i = 0; i < oldest->memunits && remainingsize > 0; i++)
        {
            if (oldest->memory[i]->allocated)
            {
                if (pageid == -1){
                    pageid = oldest->memory[i]->id;
                }
                oldest->memory[i]->allocated = 0;
                remainingsize -= oldest->memory[i]->size;
            }
        }
    }
    p->id = pageid;
    return time;
}

// assign_memory assigns memory to a process
int assign_memory (mem memory, queue *q, process *next, int quantum, int time)
{
    for (int i = 0; i < next->memsize; i++)
    {
        page *p = next->memory[i];
        if (p->allocated == 0)
        {
            time = memoryallocate (memory,q, p, time);
        }
    }
    return time;
}

int apply_quantum (process *head, process *next, int quantum, int time)
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
