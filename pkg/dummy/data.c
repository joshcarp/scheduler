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
    d->next = NULL;
    // strncpy (d->key, key, 20);
    d->key = atoi (fields[0]);
    d->arrival = atoi (fields[0]);
    d->jobtime = atoi (fields[3]);
    d->procid = atoi (fields[1]);
    d->memsize = atoi (fields[2]);

    return d;
}

/* freeData recursivley frees data */
void freeData (datat *d)
{
    assert (d);
    if (d->next != NULL)
    {
        freeData (d->next);
        d->next = NULL;
    }
    free (d);
}

/*blankData returns pointer to empty data struct */
datat *blankData ()
{
    datat *d = (datat *)malloc (sizeof (datat));
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
            if (next->next == NULL || next->next->procid >= e->procid)
            {
                tmp = next->next;
                next->next = e;
                e->next = tmp;
                set = 1;
                break;
            }
        }
        next = next->next;
    }
    if (!set)
    {
        e->next = d;
        return e;
    }
    return d;
}

// /* searchData finds and prints when found searchQuery in position fieldNum of csv file' */
// int searchData (datat *d, int fieldNum, FILE *outFile, char *searchQuery)
// {
//     dictKey_t attribute;
//     int numComps = 0;

//     while (d)
//     {
//         // get the field and compare to our searchQuery
//         if (cmp (getField (d, fieldNum, attribute), searchQuery) == EQ)
//         {
//             printFAttr (d, outFile, attribute);
//         }
//         d = d->next;
//         numComps++;
//     }
//     return numComps;
// }

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
    // fseek (d->file, d->location, SEEK_SET);
    // char buf[512];
    // fgets_noNL (buf, 1024, d->file);
    // fprintf (outFile, "time: %s", d->key);
    // char *field = strtok (buf, " ");
    // fprintf (outFile, "procID: %s", field);
    // field = strtok (NULL, " ");
    // fprintf (outFile, "mem size: %s", field);
    // field = strtok (NULL, " ");
    // fprintf (outFile, "jobtime: %s", field);
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