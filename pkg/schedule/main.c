
#include "scheduler.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#define PAGE_LENGTH 4

// least_remaining_time returns true if the first element has less time remaining than the second
bool least_remaining_time (process *d, process *t)
{
    return (d->remaining < t->remaining);
}
/* run runs the processing algorithms on head */
int run (process *head, int quantum, int memory_size, enum memory_algorithm mem_algo, enum scheduler_algorithms schedule)
{
    int time = 0;
    queue *q = new_q ();
    int remaining = left (head, -1);
    if (head == NULL)
    {
        return time;
    }

    process *next = NULL;
    process *data = head;
    mem *memory = new_memory (memory_size / PAGE_LENGTH);
    int loadtime = 0;
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
                if (schedule == custom_schedule)
                {
                    add_sorted (q, data, least_remaining_time);
                }
                else
                {
                    add (q, data);
                }
            }
            data = data->llNext;
        }
        if (next != NULL && next->remaining != 0)
        {
            if (schedule == custom_schedule)
            {
                add_sorted (q, next, least_remaining_time);
            }
            else
            {
                add (q, next);
            }
        }
        else
        {
            next = NULL;
        }
        next = pop (q);
        if (next != NULL)
        {
            if (mem_algo != unlimited)
            {
                loadtime = assign_memory (memory, q, next, time, mem_algo);
            }
            time = apply_quantum (memory, head, next, quantum, time, loadtime, schedule);
            remaining = left (head, -1);
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
int assign_memory (mem *memory, queue *q, process *next, int time, enum memory_algorithm type)
{
    int loaded = loaded_pages (next->memory);
    int needed_pages = next->memory->cap - loaded;
    int non_page_fault = 4 - loaded;
    int allocated_pages = next->memory->cap;
    int loadtime = 0;
    if (type == virtual && (memory->cap - memory->len < needed_pages))
    {
        next->remaining += needed_pages - non_page_fault; // page faults
        needed_pages = non_page_fault;
        allocated_pages = non_page_fault;
    }
    for (int i = 0; i < allocated_pages; i++)
    {
        page *p = next->memory->pages[i];
        if (p->allocated == false)
        {
            if (memoryallocate (memory, p)) // attempt to assign without evicting processes
            {
                next->loadtime += 2;
                loadtime += 2; // TODO: remove one of these
                continue;
            }
            process *oldest = q->front; // the front of the queue is this process
            bool searching = true;
            do
            {
                if (type == swapping) // swapping
                {
                    if (evict_process (memory, oldest))
                    {
                        needed_pages -= oldest->memory->cap;
                        searching = false;
                    }
                }
                else if (type == virtual)
                {
                    for (int i = 0; i < oldest->memory->cap && needed_pages > 0; i++)
                    {
                        if (evict_page (memory, oldest->memory->pages[i]))
                        {
                            searching = false;
                            needed_pages--;
                        }
                    }
                }
                oldest = oldest->queueNext;
            } while (needed_pages > 0 && searching && oldest != NULL);
            assert (memoryallocate (memory, p));
            next->loadtime += 2;
            loadtime += 2;
        }
    }
    print_evicted (memory, time);
    memory->num_recently_evicted = 0;
    return loadtime;
}

/* apply_quantum applies a quantum to a process */
int apply_quantum (mem *memory, process *head, process *next, int quantum, int time, int loadtime, enum scheduler_algorithms type)
{
    if (next->remaining == 0)
    {
        return time;
    }
    if (next->loadtime != 0)
    {
        printf ("%d, RUNNING, id=%d, remaining-time=%d, load-time=%d, mem-usage=%.f%%, ", time, next->procid,
                next->remaining, loadtime, ceiling (((float)memory->len / memory->cap) * (float)100));
        print_addresses (next->memory->pages, next->memory->cap, true);
        printf ("\n");
        time += loadtime;
    }
    else
    {
        printf ("%d, RUNNING, id=%d, remaining-time=%d\n", time, next->procid, next->remaining);
    }
    if (type == first_come)
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
        time += next->remaining;
        next->remaining = 0;
    }
    evict_process (memory, next);
    print_evicted (memory, time);
    memory->num_recently_evicted = 0;
    next->finishingtime = time;
    printf ("%d, FINISHED, id=%d, proc-remaining=%d\n", time, next->procid, left (head, time));
    return time;
}

int left (process *head, int time)
{
    if (head == NULL)
    {
        return 0;
    }
    int count = 0;
    process *data = head;
    while (data)
    {
        count += data->remaining != 0 && (data->arrival <= time || time == -1);
        data = data->llNext;
    }
    return count;
}
