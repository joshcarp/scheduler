
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
int ceiling (float x)
{
    float rem = x - (int)x;
    if (rem > 0.5)
    {
        return 1 + x;
    }
    return x;
}
int next (datat *head, enum scheduler type, int quantum, int memory_size, enum memory_algorithm malgo, enum scheduler schedule)
{
    int time = 0;
    // int remaining = left (head);
    // do
    // {
    time = next_helper (head, type, quantum, memory_size, time, malgo, schedule);
    //     remaining = left (head);
    // } while (remaining);
    return 0;
}

void printStats (datat *head, int time)
{
    int intervals = ceiling (((float)time / 60));
    int *throughput = (int *)calloc (intervals, sizeof (int) * (intervals));
    int total_turnaroundtime = 0;
    float time_overhead_max = 0;
    float time_overhead_total = 0;
    float overhead;
    int num = 0;
    datat *next = head;
    while (next)
    {
        throughput[next->finishingtime / 60] += 1;
        total_turnaroundtime += next->finishingtime - next->arrival;
        overhead = (float)(next->finishingtime - next->arrival) / next->jobtime;
        if (overhead > time_overhead_max)
        {
            time_overhead_max = overhead;
        }
        time_overhead_total += overhead;
        num++;
        next = next->llNext;
    }

    int max_throughput = -1;
    int min_throughput = -1;
    float ave_throughput = (float)num / intervals;
    for (int i = 0; i < intervals; i++)
    {
        if (throughput[i] > max_throughput || max_throughput == -1)
        {
            max_throughput = throughput[i];
        }
        else if (throughput[i] < min_throughput || min_throughput == -1)
        {
            min_throughput = throughput[i];
        }
    }
    printf ("Throughput %d %d %d\n", ceiling (ave_throughput), min_throughput, max_throughput);
    printf ("Turnaround time %d\n", total_turnaroundtime / num);
    printf ("Time overhead %.2f, %.2f\n", time_overhead_max, time_overhead_total / num);
    printf ("Makespan %d\n", time);
    free (throughput);
    freeData (head);
}

int next_helper (datat *head, enum scheduler type, int quantum, int memory_size, int time, enum memory_algorithm malgo, enum scheduler schedule)
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
    memory.recently_evicted = (page **)calloc (memory.len, sizeof (page *));
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
            if (schedule != unlimited)
            {
                time = assign_memory (&memory, q, next, quantum, time, malgo);
            }
            time = apply_quantum (&memory, head, next, quantum, time, schedule);


            remaining = left (head);
        }
    }
    printStats (head, time);
    free (q);
    free (memory.memory);
    free (memory.recently_evicted);
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
        memory->recently_evicted[memory->num_recently_evicted++] = next;
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

void printAddresses (page **arr, int n)
{
    printf ("mem-addresses=[");
    for (int i = 0; i < n; i++)
    {
        printf ("%d", arr[i]->id);
        if (i != n - 1)
        {
            printf (", ");
        }
    }
    printf ("]");
}
void printevicted (mem *memory, int time)
{
    if (memory->num_recently_evicted > 0)
    {
        printf ("%d, EVICTED, ", time);
        printAddresses (memory->recently_evicted, memory->num_recently_evicted);
        printf ("\n");
    }
}

// assign_memory assigns memory to a process
int assign_memory (mem *memory, queue *q, datat *next, int quantum, int time, enum memory_algorithm type)
{
    int needed_pages = next->memunits;

    for (int i = 0; i < needed_pages; i++)
    {
        page *p = next->memory[i];
        if (p->allocated == false)
        {
            if (memoryallocate (memory, q, p)) // attempt to assign without evicting processes
            {
                next->loadtime += 2;
                continue;
            }
            datat *oldest = q->front; // the front of the queue is this process, so the one following is the last executed.
            bool searching = true;
            do
            {
                if (type == swapping) // swapping
                {
                    if (evict_process (memory, oldest))
                    {
                        needed_pages -= oldest->memunits;
                        searching = false;
                    }
                }
                else if (type == virtual)
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
            next->loadtime += 2;
        }
    }
    printevicted (memory, time);
    return time;
}

int apply_quantum (mem *memory, datat *head, datat *next, int quantum, int time, enum scheduler type)
{
    if (next->remaining == 0)
    {
        return time;
    }
    if (next->loadtime != 0)
    {
        printf ("%d, RUNNING, id=%d, remaining-time=%d, load-time=%d, mem-usage=%d%%, ", time,
                next->procid, next->remaining, next->loadtime, (memory->cap / memory->len) * 100);
        printAddresses (next->memory, next->memunits);
        printf ("\n");
        time += next->loadtime;
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
        time += quantum - next->remaining;
        next->remaining = 0;
    }
    evict_process (memory, next);
    printevicted (memory, time);
    memory->num_recently_evicted = 0;
    next->finishingtime = time;
    printf ("%d, FINISHED, id=%d, proc-remaining=%d\n", time, next->procid, left (head));
    return time;
}
