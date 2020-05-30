
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

int next (datat *head, enum scheduler type, int quantum, int memory_size)
{
    int time = 0;
    int remaining = left (head);
    do
    {
        time = next_helper (head, type, quantum, time, memory_size);
        remaining = left (head);
    } while (remaining);
    return 0;
}

int next_helper (datat *head, enum scheduler type, int quantum, int time, int memory_size)
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
    memory.len = memory_size / MEMLEN;
    memory.memory = (page **)calloc (memory.len, sizeof (page *));
    assert (memory.memory);
    memory.recently_evicted = (int *)calloc (memory.len, sizeof (int) * memory.len);
    assert (memory.recently_evicted);
    memory.num_recently_evicted = 0;
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
        if (next != NULL)
        {
            time = assign_memory (&memory, q, next, quantum, time);
            time = apply_quantum (&memory, head, next, quantum, time);


            remaining = left (head);
        }
    }
    return time;
}

bool memoryallocate (mem *memory, queue *q, page *p)
{
    int pageid = -1;
    for (int i = 0; i < memory->len; i++)
    {
        if (memory->memory[i] == NULL)
        {
            memory->memory[i] = p;
            memory->cap++;
            p->allocated = true;
            if (pageid == -1)
            {
                p->id = i;
            }
            return true;
        }
    }
    return false;
}

bool evict_page (mem *memory, page *next)
{
    if (next->allocated)
    {
        memory->memory[next->id] = NULL;
        next->allocated = false;
        memory->recently_evicted[memory->num_recently_evicted++] = next->id;
        memory->cap--;
        return true;
    }
    return false;
}

bool evict_process (mem *memory, datat *next)
{
    bool success = false;
    for (int i = 0; i < next->memunits; i++)
    {
        if (next->memory[i]->allocated)
        {
            success = evict_page (memory, next->memory[i]);
        }
    }
    return success;
}


int *getIds (datat *next)
{
    int *ids = (int *)calloc (next->memunits, sizeof (int) * next->memunits);
    for (int i = 0; i < next->memunits; i++)
    {
        ids[i] = next->memory[i]->id;
    }
    return ids;
}
// assign_memory assigns memory to a process
int assign_memory (mem *memory, queue *q, datat *next, int quantum, int time)
{
    int needed_pages = next->memunits;

    for (int i = 0; i < needed_pages; i++)
    {
        page *p = next->memory[i];
        if (p->allocated == false)
        {
            if (memoryallocate (memory, q, p)) // attempt to assign without evicting processes
            {
                time += 2;
                continue;
            }
            datat *oldest = q->front; // the front of the queue is this process, so the one following is the last executed.
            bool searching = true;
            do
            {
                if (MEMALGO == 0) // swapping
                {
                    if (evict_process (memory, oldest))
                    {
                        needed_pages -= oldest->memunits;
                        searching = false;
                    }
                }
                else // virtual
                {
                    for (int i = 0; i < oldest->memunits && needed_pages > 0; i++)
                    {
                        if (evict_page (memory, oldest->memory[i]))
                        {
                            searching = false;
                            needed_pages--;
                        }
                    }
                }
                oldest = oldest->queueNext;
            } while (searching && oldest != NULL);
            assert (memoryallocate (memory, q, p));
            time += 2;
        }
    }
    if (memory->num_recently_evicted > 0)
    {
        printf ("%d, EVICTED, mem-addresses=[", time);
        for (int i = 0; i < memory->num_recently_evicted; i++)
        {
            printf ("%d", memory->recently_evicted[i]);
            if (i != memory->num_recently_evicted - 1)
            {
                printf (", ");
            }
        }
        memory->num_recently_evicted = 0;
        printf ("]\n");
    }
    return time;
}

int apply_quantum (mem *memory, datat *head, datat *next, int quantum, int time)
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
    evict_process (memory, next);
    printf ("%d, FINISHED, id=%d, proc-remaining=%d\n", time, next->procid, left (head));
    return time;
}
