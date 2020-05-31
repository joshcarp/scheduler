/*
This code written by Joshua Carpeggiani
data.c contains the data struct and all supporting files
program can be changed to support completely different keys/data entries only by changing the code in this file
*/

#include "data.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PAGE_LENGTH 4
#define TOTALMEM 400
#ifndef DATA_H
#define DATA_H
#endif


// cmp compares keys and returns:
//
//   LT, or -1 if m <  n
//   EQ, or 0 if m == n
//   GT, or +1 if m >  n
//
int cmp (int m, int n)
{
    if (m < n)
    {
        return LT;
    }
    else if (m == n)
    {
        return EQ;
    }
    return GT;
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

page *new_page ()
{
    page *p = (page *)malloc (sizeof (page));
    assert (p);
    p->allocated = false;
    p->id = -1;
    p->size = PAGE_LENGTH;
    return p;
}
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
    free (d);
}

/*blankData returns pointer to empty data struct */
process *blankData ()
{
    process *d = (process *)malloc (sizeof (process));
    d->llNext = NULL;
    d->queueNext = NULL;
    d->queuePrev = NULL;
    assert (d);
    return d;
}

// Link data links the data together in a linked list
process *linkData (process *d, process *e)
{
    assert (d);
    process *tmp;
    // look, I would insert this right at the head but it's still O(1) insertion
    process *next = d;
    int set = 0;
    while (next)
    {
        if (next->procid <= e->procid)
        {
            if (next->llNext == NULL || next->llNext->procid >= e->procid)
            {
                tmp = next->llNext;
                next->llNext = e;
                e->llNext = tmp;
                set = 1;
                break;
            }
        }
        next = next->llNext;
    }
    if (!set)
    {
        e->llNext = d;
        return e;
    }
    return d;
}

/* fieldFromStr adds a new node to the bst by parsing the row and keeping track of the location within the file */
void fieldFromStr (char *buff, char dest[MAXFIELDNUM][MAXFIELD])
{
    int i = 0;
    char *token;
    token = strtok (buff, " ");
    do
    {
        strcpy (dest[i], token);
        token = strtok (NULL, " ");
        i++;
    } while (token);
}


/*printFData is the function used to print the contents of data */
void printFData (process *d, FILE *outFile)
{
    if (!outFile)
    {
        printData (d);
        return;
    }
    return;
}

/*printFAttr is the function used to print the Attribute search to file */
void printFAttr (process *d, FILE *outFile, char *Attr)
{
    if (!outFile)
    {
        printData (d);
        return;
    }

    fprintf (outFile, "%s --> %d\n", Attr, d->key);
    return;
}
/*printData is the function used to print the contents of data */
void printData (process *d)
{
    printf ("%d", d->key);
}

queue *new_q ()
{
    queue *q = (struct queue *)malloc (sizeof (struct queue));
    q->front = q->rear = NULL;
    q->num = 0;
    return q;
}

void add (queue *q, process *d)
{
    struct process *temp = d;
    q->num++;

    struct process *t = q->front;
    while (t)
    {
        assert (t != d);
        t = t->queueNext;
    }

    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL)
    {
        q->front = q->rear = temp;
        return;
    }

    // Add the new node at the end of queue and change rear
    q->rear->queueNext = temp;
    q->rear = temp;
}

process *pop (queue *q)
{
    if (q->front == NULL || q->num == 0)
    {
        return NULL;
    }

    process *temp = q->front;
    q->front = q->front->queueNext;
    if (q->front == NULL)
    {
        q->rear = NULL;
    }
    q->num--;
    temp->queueNext = NULL;
    return temp;
}

// /* parseFile parses a csv file into a bst. cmd is for command line arguments*/
process *parseFile (FILE *file)
{
    char buf[MAXBUFF];
    int buffSize = 0;
    process *head = NULL;
    process *d;
    process *next = NULL;
    // While stdin, keep cumulative sum of location from start of file and parse row into the bst
    while (fgets (buf, MAXBUFF, file) && buf[0] != '\n')
    {
        buffSize = strlen (buf);
        buf[buffSize] = '\0';
        d = newData (buf);
        if (next == NULL)
        {
            next = d;
            head = d;
        }
        else
        {
            next->llNext = d;
            next = d;
        }
    }
    return head;
}