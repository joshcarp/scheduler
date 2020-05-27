/* By Joshua Carpeggiani
This file conatins everything to do with parsing and making the binary search tree,
as well as code for the randomising input*/

#include "bst.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* newNode inserts data into a new node and returns the new node */
node_t *newNode (datat *data)
{
    node_t *node = blankNode ();
    node->data = data;
    return node;
}

/* blankNode returns a new empty node */
node_t *blankNode ()
{
    node_t *node = (node_t *)malloc (sizeof (node_t));
    assert (node);
    node->gt = NULL;
    node->lt = NULL;
    return node;
}

/* freeTree recursively frees the nodes and data in a tree */
void freeTree (node_t *parent)
{
    if (parent == NULL) return;
    if (parent->gt != NULL)
    {
        freeTree (parent->gt);
        parent->gt = NULL;
    }
    if (parent->lt != NULL)
    {
        freeTree (parent->lt);
        parent->lt = NULL;
    }
    freeData (parent->data);
    free (parent);
    return;
}

/* bstInsert inserts a data pointer into a tree */
void bstInsert (node_t *parent, datat *data)
{
    node_t **insertAt;
    int comparison;
    if (parent->data == NULL)
    {
        parent->data = data;
        return;
    }

    while (parent != NULL)
    {
        comparison = cmp (data->key, parent->data->key);
        if (comparison == LT)
        {
            insertAt = &parent->lt;
            parent = parent->lt;
        }
        else if (comparison == GT)
        {
            insertAt = &parent->gt;
            parent = parent->gt;
        }
        else
        {
            parent->data = linkData (parent->data, data);
            return;
        }
    }
    *insertAt = newNode (data);
}

/* parseFile parses a csv file into a bst. cmd is for command line arguments*/
datat *parseFile (FILE *file)
{
    char buf[MAXBUFF];
    int currentloc = 0;
    int buffSize = 0;
    int rowcount = 0;
    datat *head = NULL;
    datat *d;
    datat *next = NULL;
    // While stdin, keep cumulative sum of location from start of file and parse row into the bst
    while (fgets (buf, MAXBUFF, file) && buf[0] != '\n')
    {
        buffSize = strlen (buf);
        buf[buffSize] = '\0';
        if (rowcount < NUMSKIPPEDWROWS)
        {
            rowcount++;
            currentloc += buffSize;
            continue;
        }

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

datat *toList_helper (node_t *head, datat *parent)
{
    if (head == NULL)
    {
        return parent;
    }
    toList_helper (head->lt, parent);
    datat *data = parent;
    while (data->llNext)
    {
        data = data->llNext;
    }
    data->llNext = head->data;
    toList_helper (head->gt, parent);
    return parent;
}

datat *toList (node_t *head)
{
    datat *parent = blankData ();
    toList_helper (head, parent);
    return parent->llNext;
}
