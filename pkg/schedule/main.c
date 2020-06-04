
/* main.c has all of the "exported" functions that are called from the main; run is called with the parsed fields from the cli */
#include "scheduler.h"
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#define PAGE_LENGTH 4
#define OLD_WEIGHT 0.1


/* least_remaining_time returns true if the first element has less time remaining than the second, but also puts
   a weight to how long a process has lasted, so that every job will eventually finish */
bool least_remaining_time (process *d, process *t)
{
    uint time = d->arrival > d->last_execution_time ?
                d->arrival :
                d->last_execution_time; // time is either arrival or last_execution_time

    return (d->remaining + OLD_WEIGHT * (time - d->arrival) < t->remaining + OLD_WEIGHT * (time - t->arrival));
}

/* run runs the processing algorithms on head */
int run (process *head, uint quantum, uint memory_size, enum memory_algorithm mem_algo, enum scheduler_algorithms schedule)
{
    uint time = 0;
    uint remaining = left (head, -1);
    uint loadtime = 0;
    queue *q = new_q ();
    process *next = NULL;
    process *data = head;
    mem *memory = new_memory (memory_size / PAGE_LENGTH);


    /* Use a function pointer to determine which type of insert we're gonna do */
    bool (*sorting_func) (process * d, process * t) = NULL;
    if (schedule == custom_schedule)
    {
        sorting_func = least_remaining_time;
    }

    /* This determines which function will be used by the memory manager */
    uint (*eviction_function) (mem * memory, process * head, uint needed_pages);
    switch (mem_algo)
    {
    case swapping:
        eviction_function = swapping_memory_evict;
        break;
    case virtual:
        eviction_function = virtual_memory_evict;
        break;
    case custom_memory:
        eviction_function = custom_memory_evict;
        break;
    default:
        break;
    }

    /*  */
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
                add_sorted (q, data, sorting_func);
            }
            data = data->llNext;
        }
        if (next != NULL && next->remaining != 0)
        {
            add_sorted (q, next, sorting_func);
        }
        next = pop (q);
        if (next != NULL)
        {
            if (mem_algo == unlimited)
            {
                printf ("%u, RUNNING, id=%u, remaining-time=%u\n", time, next->procid, next->remaining);
            }
            else
            {
                loadtime = assign_memory (memory, q, next, time, eviction_function);
                printf ("%u, RUNNING, ", time);
                printf ("id=%u, ", next->procid);
                printf ("remaining-time=%u, ", next->remaining);
                printf ("load-time=%u, ", loadtime);
                printf ("mem-usage=%.f%%, ", ceiling (((float)memory->len / memory->cap) * (float)100));
                print_addresses (next->memory->pages, next->memory->cap, true);
                printf ("\n");
                time += loadtime;
            }
            time = apply_quantum (memory, head, next, quantum, time, schedule);
            remaining -= next != NULL && next->remaining == 0;
        }
        else
        {
            time++;
        }
    }
    print_stats (head, time);
    free (q);
    free (memory->pages);
    free (memory->recently_evicted);
    free (memory);
    return 0;
}


/* assign_memory assigns memory to a process */
uint assign_memory (mem *memory, queue *q, process *next, uint time, uint (*evict) (mem *, process *, uint))
{
    uint loaded = loaded_pages (next->memory);
    uint to_be_evicted = next->memory->cap - loaded;
    uint to_be_allocated = next->memory->cap;
    uint loadtime = 0;

    // if there's not enough space in out memory, then we need to only get 4 pages loaded (the rest are page faults)
    if (evict != swapping_memory_evict && memory->cap - memory->len < to_be_evicted)
    {
        int limit = 4 - loaded;
        next->remaining += to_be_evicted - limit; // page faults;
        to_be_evicted = limit;
        to_be_allocated = limit;
    }

    if (memory->cap - memory->len < to_be_evicted) // if there isn't enough space, then we need to find enough space
    {
        to_be_evicted = evict (memory, q->front, to_be_evicted);
    }

    uint allocated = 0;
    for (uint i = 0; i < next->memory->cap && allocated < to_be_allocated; i++)
    {
        page *p = next->memory->pages[i];
        if (p->allocated)
        {
            continue;
        }
        if (memoryallocate (memory, p))
        {
            next->memory->len++;
            next->loadtime += 2;
            loadtime += 2;
            allocated++;
        }
    }
    print_evicted (memory, time);
    memory->num_recently_evicted = 0;
    return loadtime;
}

/* apply_quantum applies a quantum to a process */
int apply_quantum (mem *memory, process *head, process *next, uint quantum, uint time, enum scheduler_algorithms type)
{
    if (quantum > next->remaining)
    {
        time += next->remaining;
        next->remaining = 0;
    }
    else if (quantum < next->remaining)
    {
        time += quantum;
        next->remaining -= quantum;
        next->last_execution_time = time;
        return time;
    }
    else
    {
        time += quantum;
        next->remaining = 0;
    }

    evict_process (memory, next);
    print_evicted (memory, time);
    memory->num_recently_evicted = 0;
    next->last_execution_time = time;
    printf ("%u, FINISHED, id=%u, proc-remaining=%u\n", time, next->procid, left (head, time));
    return time;
}
