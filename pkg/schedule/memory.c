#include "scheduler.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

/* memoryallocate allocates p in memory and returns wether the process was a success */
bool memoryallocate (mem *memory, page *p)
{
    for (uint i = 0; i < memory->cap; i++)
    {
        if (memory->pages[i] == NULL)
        {
            memory->pages[i] = p;
            memory->len++;
            p->allocated = true;
            p->id = i;
            return true;
        }
    }
    return false;
}

/* evict_page evicts a page from memory */
bool evict_page (mem *memory, page *next)
{
    if (next->allocated)
    {
        memory->pages[next->id] = NULL;
        next->allocated = false;
        memory->recently_evicted[memory->num_recently_evicted++] = next;
        memory->len--;
        return true;
    }
    return false;
}

/* evict_process evicts a process from memory */
bool evict_process (mem *memory, process *next)
{
    bool success = false;
    for (uint i = 0; i < next->memory->cap; i++)
    {
        if (next->memory->pages[i]->allocated)
        {
            success = evict_page (memory, next->memory->pages[i]);
            next->memory->len -= success;
        }
    }
    return success;
}

/* loaded_pages returns the amound of loaded pages */
int loaded_pages (mem *memory)
{
    int loaded = 0;
    for (uint i = 0; i < memory->cap; i++)
    {
        if (memory->pages[i]->allocated)
        {
            loaded++;
        }
    }
    return loaded;
}


/* evict_upto evicts needed pages from to_evict */
int evict_upto (mem *memory, mem *to_evict, int needed_pages)
{
    for (uint i = 0; i < to_evict->cap && needed_pages > 0; i++)
    {
        if (evict_page (memory, to_evict->pages[i]))
        {
            needed_pages--;
            to_evict->len--;
        }
    }
    return needed_pages;
}

/* virtual_memory_evict takes in a head of the queue and evicts upto needed_pages from the oldest process*/
int virtual_memory_evict (mem *memory, process *head, int needed_pages)
{
    while (head && needed_pages > 0)
    {
        needed_pages = evict_upto (memory, head->memory, needed_pages);
        head = head->queueNext;
    }
    return needed_pages;
}


/* swapping_memory_evict evicts the current program from memory */
int swapping_memory_evict (mem *memory, process *head, int needed_pages)
{
    while (head && needed_pages > 0)
    {
        if (evict_process (memory, head))
        {
            needed_pages -= head->memory->cap;
        }
        head = head->queueNext;
    }
    return needed_pages;
}

/* custom_memory_evict evicts the command which has the longest time to complete */
int custom_memory_evict (mem *memory, process *head, int needed_pages)
{
    process *longest_left = NULL;
    while (needed_pages > 0)
    {
        longest_left = head;
        process *oldest = head;
        while (oldest)
        {
            if ((longest_left->remaining < oldest->remaining && oldest->memory->len < oldest->memory->cap))
            {
                longest_left = oldest;
            }
            oldest = oldest->queueNext;
        }
        needed_pages = evict_upto (memory, longest_left->memory, needed_pages);
    }
    return needed_pages;
}
