
#include "scheduler.h"

int left (node_t *head)
{
    if (head == NULL)
    {
        return 0;
    }
    int count = 0;
    count += left (head->lt);
    datat *data = head->data;
    while (data)
    {
        count += data->remaining != 0;
        data = data->next;
    }
    count += left (head->gt);
    return count;
}

int next (node_t *head, enum scheduler type, int quantum, int time)
{
    while (left (head))
    {
        next_helper (head, type, quantum, time);
    }
    return 0;
}
int next_helper (node_t *head, enum scheduler type, int quantum, int time)
{
    if (head == NULL)
    {
        return time;
    }
    time = next (head->lt, type, quantum, time);
    datat *data = head->data;
    while (data)
    {
        time = apply_quantum (data, quantum, time);
        data = data->next;
    }
    time = next (head->gt, type, quantum, time);
    return time;
}


int apply_quantum (datat *next, int quantum, int time)
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
    else if (quantum <= next->remaining)
    {
        time += quantum;
        next->remaining -= quantum;
        return time;
    }
    else
    {
        time += quantum - next->remaining;
        next->remaining = 0;
    }
    printf ("%d, FINISHED, id=%d, proc-remaining=%d\n", time, next->procid, 9999);
    return time;
}
