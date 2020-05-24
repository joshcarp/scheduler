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
int cmp (char *m, char *n)
{
    int comparison = strcmp (m, n);

    if (comparison < 0)
    {
        return LT;
    }
    else if (comparison == 0)
    {
        return EQ;
    }
    return GT;
}

/*newData returns a datat pointer with num as key */
datat *newData (char *key, int location, FILE *file)
{
    datat *d = blankData ();
    d->next = NULL;
    strncpy (d->key, key, 20);
    d->location = location;
    d->file = file;
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
void linkData (datat *d, datat *e)
{
    assert (d);
    datat *tmp;
    // look, I would insert this right at the head but it's still O(1) insertion
    tmp = d->next;
    d->next = e;
    e->next = tmp;
}

/* searchData finds and prints when found searchQuery in position fieldNum of csv file' */
int searchData (datat *d, int fieldNum, FILE *outFile, char *searchQuery)
{
    dictKey_t attribute;
    int numComps = 0;

    while (d)
    {
        // get the field and compare to our searchQuery
        if (cmp (getField (d, fieldNum, attribute), searchQuery) == EQ)
        {
            printFAttr (d, outFile, attribute);
        }
        d = d->next;
        numComps++;
    }
    return numComps;
}

/* fieldFromStr adds a new node to the bst by parsing the row and keeping track of the location within the file */
char *fieldFromStr (char *row, int fieldNum, char *dest)
{
    char buff[MAXFIELD];
    int i = 1;
    char *token;

    strcpy (buff, row);
    token = strtok (buff, " ");
    while (i <= fieldNum)
    {
        token = strtok (NULL, " ");
        i++;
    }
    strcpy (dest, token);
    return dest;
}

// getField returns the string of fieldnum from the file it came from
char *getField (datat *d, int fieldNum, char *dest)
{
    fseek (d->file, d->location, SEEK_SET);
    char buf[MAXBUFF];
    fgets_noNL (buf, MAXBUFF, d->file);
    // buf[strcspn(buf, "\n")] = '\0';
    return fieldFromStr (buf, fieldNum, dest);
}


/*printFData is the function used to print the contents of data */
void printFData (datat *d, FILE *outFile)
{
    if (!outFile)
    {
        printData (d);
        return;
    }
    fseek (d->file, d->location, SEEK_SET);
    char buf[512];
    fgets_noNL (buf, 1024, d->file);
    fprintf (outFile, "%s --> ", d->key);
    char *field = strtok (buf, ",");
    fprintf (outFile, "VendorID: %s ||", field);
    field = strtok (NULL, ",");
    fprintf (outFile, " passenger_count: %s ||", field);
    field = strtok (NULL, ",");
    fprintf (outFile, " trip_distance: %s ||", field);
    field = strtok (NULL, ",");
    fprintf (outFile, " RatecodeID: %s ||", field);
    field = strtok (NULL, ",");
    fprintf (outFile, " store_and_fwd_flag: %s ||", field);
    field = strtok (NULL, ",");
    fprintf (outFile, " PULocationID: %s ||", field);
    field = strtok (NULL, ",");
    fprintf (outFile, " DOLocationID: %s ||", field);
    field = strtok (NULL, ",");
    fprintf (outFile, " payment_type: %s ||", field);
    field = strtok (NULL, ",");
    fprintf (outFile, " fare_amount: %s ||", field);
    field = strtok (NULL, ",");
    fprintf (outFile, " extra: %s ||", field);
    field = strtok (NULL, ",");
    fprintf (outFile, " mta_tax: %s ||", field);
    field = strtok (NULL, ",");
    fprintf (outFile, " tip_amount: %s ||", field);
    field = strtok (NULL, ",");
    fprintf (outFile, " tolls_amount: %s ||", field);
    field = strtok (NULL, ",");
    fprintf (outFile, " improvement_surcharge: %s ||", field);
    field = strtok (NULL, ",");
    fprintf (outFile, " total_amount: %s ||", field);
    field = strtok (NULL, ",");
    field = strtok (NULL, ",");
    fprintf (outFile, " DOdatetime: %s ||", field);
    field = strtok (NULL, ",");
    fprintf (outFile, " trip_duration: %s ||\n", field);
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

    fprintf (outFile, "%s --> %s\n", Attr, d->key);
    return;
}
/*printData is the function used to print the contents of data */
void printData (datat *d)
{
    printf ("%s", d->key);
}

/*printKey is the function used to print the contents of data */
void printKey (char *k)
{
    printf ("%s-> \n", k);
}

// fgets_noNL does an fgets but replaces the newline with a \0
char *fgets_noNL (char *str, int n, FILE *stream)
{
    char *buf = fgets (str, n, stream);
    buf[strcspn (buf, "\n")] = '\0';
    return buf;
}

dictKey_t *NewKey ()
{
    return (dictKey_t *)malloc (sizeof (dictKey_t));
}