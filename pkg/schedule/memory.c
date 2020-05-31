#include "scheduler.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

/* memoryallocate allocates p in memory and returns wether the process was a success */
bool memoryallocate (mem *memory, page *p)
{
    for (int i = 0; i < memory->cap; i++)
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
    for (int i = 0; i < next->memory->cap; i++)
    {
        if (next->memory->pages[i]->allocated)
        {
            success = evict_page (memory, next->memory->pages[i]);
        }
    }
    return success;
}

/* loaded_pages returns the amound of loaded pages */
int loaded_pages (mem *memory)
{
    int loaded = 0;
    for (int i = 0; i < memory->cap; i++)
    {
        if (memory->pages[i]->allocated)
        {
            loaded++;
        }
    }
    return loaded;
}
