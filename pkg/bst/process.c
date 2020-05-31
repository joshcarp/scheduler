#include "structures.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PAGE_LENGTH 4
#define TOTALMEM 400
#ifndef DATA_H
#define DATA_H
#endif

/*newData returns a process pointer with num as key */
process *newData (char *entry)
{
    char fields[MAXFIELDNUM][MAXFIELD];
    fieldFromStr (entry, fields);
    process *d = blankData ();
    d->llNext = NULL;
    d->queueNext = NULL;
    d->arrival = atoi (fields[0]);
    d->jobtime = atoi (fields[3]);
    d->procid = atoi (fields[1]);
    d->remaining = d->jobtime;
    d->loadtime = 0;
    d->finishingtime = 0;
    d->memory = new_memory (atoi (fields[2]) / PAGE_LENGTH);
    for (int i = 0; i < d->memory->cap; i++)
    {
        d->memory->pages[i] = new_page ();
        d->memory->pages[i]->parent = d;
    }
    return d;
}

/* freeData recursivley frees data */
void freeData (process *d)
{
    assert (d);
    if (d->llNext != NULL)
    {
        freeData (d->llNext);
        d->llNext = NULL;
    }
    for (int i = 0; i < d->memory->cap; i++)
    {
        free (d->memory->pages[i]);
    }
    free (d->memory->pages);
    free (d->memory->recently_evicted);
    free (d->memory);
    free (d);
}


/*blankData returns pointer to empty data struct */
process *blankData ()
{
    process *d = (process *)malloc (sizeof (process));
    d->llNext = NULL;
    d->queueNext = NULL;
    assert (d);
    return d;
}

mem *new_memory (int capacity)
{
    mem *memory = (mem *)malloc (sizeof (mem));
    memory->cap = capacity;
    memory->len = 0;
    memory->pages = (page **)calloc (memory->cap, sizeof (page *));
    assert (memory->pages);
    memory->recently_evicted = (page **)calloc (memory->cap, sizeof (page *));
    assert (memory->recently_evicted);
    memory->num_recently_evicted = 0;
    return memory;
}

/* new_page creates a new page */
page *new_page ()
{
    page *p = (page *)malloc (sizeof (page));
    assert (p);
    p->allocated = false;
    p->id = -1;
    p->size = PAGE_LENGTH;
    return p;
}
