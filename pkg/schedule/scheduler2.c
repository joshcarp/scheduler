
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
        data = data->next;
    }
    return count;
}

datat *toList_helper (node_t *head, datat *parent)
{
    if (head == NULL)
    {
        return parent;
    }
    toList_helper (head->lt, parent);
    datat *data = parent;
    while (data->next)
    {
        data = data->next;
    }
    data->next = head->data;
    toList_helper (head->gt, parent);
    return parent;
}

datat *toList (node_t *head)
{
    datat *parent = blankData ();
    toList_helper (head, parent);
    return parent->next;
}

int next (node_t *head, enum scheduler type, int quantum, int time)
{
    datat *this = toList (head);
    int remaining = left (this);
    do
    {

        time = next_helper (this, type, quantum, time, remaining);
        remaining = left (this);
    } while (remaining);
    return 0;
}


int next_helper (datat *head, enum scheduler type, int quantum, int time, int remaining)
{
    if (head == NULL)
    {
        return time;
    }
    datat *data = head;
    while (data)
    {
        time = apply_quantum (head, data, quantum, time);
        data = data->next;
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
    printf ("%d, FINISHED, id=%d, proc-remaining=%d\n", time, next->procid, left (head));
    return time;
}
