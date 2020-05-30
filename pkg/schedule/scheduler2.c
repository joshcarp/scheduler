
#include "scheduler.h"

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
        time = apply_quantum (head, next, quantum, time);


        remaining = left (head);
    }
    return time;
}
 //memory management here
        // time = assignmemory(next, memory, time)
        // func  assignmemory{
        //     for p = pages in next.memory{
        //         if p .allocated == no{
        //            time = memoryallocate(memory, p, time)
        //         }
        //     }
        // }
int memoryallocate(mem memory, page *p, int time){

}
int assign_memory(mem memory, datat *head, datat *next, int quantum, int time){
    for (int i = 0; i < head->memsize; i++){
        page *p = head->memory[i];
        if(p->allocated == 0){
            time = memoryallocate(memory, p, time)
        }


    }
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
