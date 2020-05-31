
#include "scheduler.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define PAGE_LENGTH 4
int left (datat *head, int time)
{
    if (head == NULL)
    {
        return 0;
    }
    int count = 0;
    datat *data = head;
    while (data)
    {
        count += data->remaining != 0 && (data->arrival <= time || time == -1);
        data = data->llNext;
    }
    return count;
}
int round_5 (float x)
{
    float rem = x - (int)x;
    if (rem > 0.5)
    {
        return 1 + x;
    }
    return x;
}

int ceiling (float x)
{
    float rem = x - (int)x;
    if (rem)
    {
        return 1 + (int)x;
    }
    return x;
}

void print_stats (datat *head, int time)
{
    int intervals = (int)(((float)time / 60));
    int *throughput = (int *)calloc (intervals, sizeof (int) * (intervals));
    int total_turnaroundtime = 1;
    float time_overhead_max = 0;
    float time_overhead_total = 0;
    float overhead;
    int num = 0;
    datat *next = head;
    while (next)
    {
        throughput[((next->finishingtime - 1) / 60)] += 1;
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
        if (throughput[i] < min_throughput || min_throughput == -1)
        {
            min_throughput = throughput[i];
        }
    }
    printf ("Throughput %d, %d, %d\n", round_5 (ave_throughput), min_throughput, max_throughput);
    printf ("Turnaround time %d\n", round_5 ((float)total_turnaroundtime / (float)num));
    printf ("Time overhead %.2f %.2f\n", time_overhead_max, time_overhead_total / num);
    printf ("Makespan %d\n", time);
    free (throughput);
    freeData (head);
}


int run (datat *head, enum scheduler type, int quantum, int memory_size, enum memory_algorithm malgo, enum scheduler schedule)
{
    int time = 0;
    queue *q = NewQueue ();
    int remaining = left (head, -1);
    if (head == NULL)
    {
        return time;
    }

    datat *next = NULL;
    datat *data = head;
    mem memory;
    memory.cap = memory_size / PAGE_LENGTH;
    memory.len = 0;
    memory.memory = (page **)calloc (memory.cap, sizeof (page *));
    assert (memory.memory);
    memory.recently_evicted = (page **)calloc (memory.cap, sizeof (page *));
    assert (memory.recently_evicted);
    memory.num_recently_evicted = 0;
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
                addToQueue (q, data);
            }
            data = data->llNext;
        }
        if (next != NULL && next->remaining != 0)
        {
            addToQueue (q, next);
        }
        else
        {
            next = NULL;
        }
        next = getFromQueue (q);
        if (next != NULL)
        {
            if (malgo != unlimited)
            {
                loadtime = assign_memory (&memory, q, next, quantum, time, malgo);
            }
            time = apply_quantum (&memory, head, next, quantum, time, loadtime, schedule);
            remaining = left (head, -1);
        }
        else
        {
            time++;
        }
    }
    print_stats (head, time);
    free (q);
    free (memory.memory);
    free (memory.recently_evicted);
    return 0;
}

bool memoryallocate (mem *memory, queue *q, page *p)
{
    // int pageid = -1;
    for (int i = 0; i < memory->cap; i++)
    {
        if (memory->memory[i] == NULL)
        {
            memory->memory[i] = p;
            memory->len++;
            p->allocated = true;
            p->id = i;
            // if (pageid == -1)
            // {

            // }
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
        memory->len--;
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

void printAddresses (page **arr, int n, bool allocated)
{
    printf ("mem-addresses=[");
    page *temp;
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            if (arr[i] != NULL && arr[j] != NULL)
            {
                if (arr[i]->id > arr[j]->id)
                {
                    temp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = temp;
                }
            }
        }
    }

    char comma[3] = "";
    for (int i = 0; i < n; i++)
    {
        if (arr[i]->id == -1 || arr[i]->allocated != allocated)
        {
            continue;
        }
        printf ("%s%d", comma, arr[i]->id);
        if (i != n - 1)
        {
            strcpy (comma, ",");
        }
    }
    printf ("]");
}
void printevicted (mem *memory, int time)
{
    if (memory->num_recently_evicted > 0)
    {
        printf ("%d, EVICTED, ", time);
        printAddresses (memory->recently_evicted, memory->num_recently_evicted, false);
        printf ("\n");
    }
}
int loaded_pages (page **arr, int n)
{
    int loaded = 0;
    for (int i = 0; i < n; i++)
    {
        if (arr[i]->allocated)
        {
            loaded++;
        }
    }
    return loaded;
}
// assign_memory assigns memory to a process
int assign_memory (mem *memory, queue *q, datat *next, int quantum, int time, enum memory_algorithm type)
{
    int loaded = loaded_pages (next->memory, next->memunits);
    int needed_pages = next->memunits - loaded;
    int non_page_fault = 4 - loaded;
    int allocated_pages = next->memunits;
    int loadtime = 0;
    // if (loaded >= 4 && memory->len == memory->cap)
    // {

    //     next->remaining += needed_pages;
    //     return 0;
    // }

    if (type == virtual && (memory->cap - memory->len < needed_pages))
    {
        // next->page_faults = needed_pages - loaded;
        // next->remaining += needed_pages - 4; // page faults
        next->remaining += needed_pages - non_page_fault; // page faults

        needed_pages = non_page_fault;
        allocated_pages = non_page_fault;
    }
    // if (needed_pages == 0)
    // {
    //     return 0;
    // }
    for (int i = 0; i < allocated_pages; i++)
    {
        page *p = next->memory[i];
        if (p->allocated == false)
        {
            if (memoryallocate (memory, q, p)) // attempt to assign without evicting processes
            {
                next->loadtime += 2;
                loadtime += 2; // TODO: remove one of these
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
            } while (needed_pages > 0 && searching && oldest != NULL);
            assert (memoryallocate (memory, q, p));
            next->loadtime += 2;
            loadtime += 2;
        }
    }
    printevicted (memory, time);
    memory->num_recently_evicted = 0;
    return loadtime;
}

int apply_quantum (mem *memory, datat *head, datat *next, int quantum, int time, int loadtime, enum scheduler type)
{
    if (next->remaining == 0)
    {
        return time;
    }
    if (next->loadtime != 0)
    {
        printf ("%d, RUNNING, id=%d, remaining-time=%d, load-time=%d, mem-usage=%d%%, ", time, next->procid,
                next->remaining, loadtime, ceiling (((float)memory->len / memory->cap) * (float)100));
        printAddresses (next->memory, next->memunits, true);
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
    printevicted (memory, time);
    memory->num_recently_evicted = 0;
    next->finishingtime = time;
    printf ("%d, FINISHED, id=%d, proc-remaining=%d\n", time, next->procid, left (head, time));
    return time;
}
