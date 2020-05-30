/*
This code written by Joshua Carpeggiani
data.c contains the data struct and all supporting files
program can be changed to support completely different keys/data entries only by changing the code in this file
*/

#include "bst.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MEMLEN 4
#define TOTALMEM 400
#ifndef DATA_H
#define DATA_H
#include "data.h"
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

/*newData returns a datat pointer with num as key */
datat *newData (char *entry)
{
    char fields[MAXFIELDNUM][MAXFIELD];
    fieldFromStr (entry, fields);
    datat *d = blankData ();
    d->llNext = NULL;
    d->queueNext = NULL;
    // strncpy (d->key, key, 20);

    d->key = atoi (fields[0]);
    d->arrival = atoi (fields[0]);
    d->jobtime = atoi (fields[3]);
    d->procid = atoi (fields[1]);
    d->memsize = atoi (fields[2]);
    d->remaining = d->jobtime;
    d->loadtime = 0;
    d->finishingtime = 0;
    d->memunits = d->memsize / MEMLEN;
    d->memory = (page **)malloc (sizeof (page *) * d->memunits);
    assert (d->memory);
    for (int i = 0; i < d->memunits; i++)
    {
        d->memory[i] = (page *)malloc (sizeof (page));
        assert (d->memory[i]);
        d->memory[i]->allocated = false;
        d->memory[i]->id = -1;
        d->memory[i]->parent = d;
        d->memory[i]->size = MEMLEN;
    }
    return d;
}

/* freeData recursivley frees data */
void freeData (datat *d)
{
    assert (d);
    if (d->llNext != NULL)
    {
        freeData (d->llNext);
        d->llNext = NULL;
    }
    free (d);
}

/*blankData returns pointer to empty data struct */
datat *blankData ()
{
    datat *d = (datat *)malloc (sizeof (datat));
    d->llNext = NULL;
    d->queueNext = NULL;
    d->queuePrev = NULL;
    assert (d);
    return d;
}

// Link data links the data together in a linked list
datat *linkData (datat *d, datat *e)
{
    assert (d);
    datat *tmp;
    // look, I would insert this right at the head but it's still O(1) insertion
    datat *next = d;
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
void printFData (datat *d, FILE *outFile)
{
    if (!outFile)
    {
        printData (d);
        return;
    }
    return;
}

/*printFAttr is the function used to print the Attribute search to file */
void printFAttr (datat *d, FILE *outFile, char *Attr)
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
void printData (datat *d)
{
    printf ("%d", d->key);
}

queue *NewQueue ()
{
    queue *q = (struct queue *)malloc (sizeof (struct queue));
    q->front = q->rear = NULL;
    return q;
}

void addToQueue (queue *q, datat *d)
{
    struct datat *temp = d;

    // If queue is empty, then new node is front and rear both
    if (q->rear == NULL)
    {
        q->front = q->rear = temp;
        return;
    }

    // Add the new node at the end of queue and change rear
    q->rear->queueNext = temp;
    temp->queuePrev = q->rear->queueNext;
    q->rear = temp;
}

datat *getFromQueue (queue *q)
{
    if (q->front == NULL)
    {
        return NULL;
    }

    datat *temp = q->front;
    q->front = q->front->queueNext;
    if (q->front != NULL)
    {
        q->front->queuePrev = NULL;
    }
    else
    {
        q->rear = NULL;
    }

    return temp;
}