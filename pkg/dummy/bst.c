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
            linkData (parent->data, data);
            return;
        }
    }
    *insertAt = newNode (data);
}

/* searchTree finds and returns the first node with key of 'key' */
node_t *searchTree (node_t *parent, char *key)
{
    int comparison = 0;
    int numComparisons = 0;

    while (parent != NULL)
    {
        comparison = cmp (key, parent->data->key);
        numComparisons++;

        if (comparison == LT)
        {
            parent = parent->lt;
        }
        else if (comparison == GT)
        {
            parent = parent->gt;
        }
        else
        {
            printf ("%s --> %d\n", key, numComparisons);
            return parent;
        }
    }

    printf ("%s --> %d\n", key, numComparisons);
    return NULL;
}

/* searchTree finds and returns the first node with key of 'key' */
void printNodes (datat *d, FILE *outFile)
{
    if (d == NULL) return;
    while (d)
    {
        printFData (d, outFile);
        d = d->next;
    }
    return;
}

/* parseFile parses a csv file into a bst. cmd is for command line arguments*/
node_t *parseFile (FILE *file, int fieldNum, int random)
{
    char buf[MAXBUFF];
    int currentloc = 0;
    int buffSize = 0;
    int rowcount = 0;
    dictKey_t key;
    node_t *head = blankNode ();
    dataList_t *dList = dataList ();
    datat *d;

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

        fieldFromStr (buf, fieldNum, key);
        d = newData (key, currentloc, file);
        if (rowcount <= NUMSKIPPEDWROWS)
        {
            head->data = d;
            rowcount++;
            continue;
        }
        if (random)
        {
            // Add data to a randomised array to deal with sorted inputs
            dataRandomise (d, dList);
        }
        else
        {
            // If the randomise flag isn't set, just directly add it
            bstInsert (head, d);
        }
        rowcount++;
        currentloc += buffSize;
    }
    // Insert the randomised data array to a bst then free the datalist
    if (random)
    {
        insertFromDataList (dList, head);
    }

    freeDataList (dList);
    return head;
}

/* parseKeys finds all keys from stdin and prints to outfile*/
void parseKeys (node_t *head, FILE *outFile)
{
    dictKey_t buf;

    while (fgets (buf, MAXFIELD, stdin))
    {
        buf[strcspn (buf, "\n")] = '\0';
        node_t *found = searchTree (head, buf);

        if (found)
        {
            printNodes (found->data, outFile);
        }
        else
        {
            fprintf (outFile, "%s -> NOTFOUND\n", buf);
        }
    }
}

/* searchTreeAttribute gets input from stdin and traverses the tree to look for attribute*/
void searchTreeAttribute (node_t *head, FILE *outFile, int fieldNum)
{
    char buf[MAXBUFF];
    int numComps;
    while (fgets (buf, MAXFIELD, stdin))
    {
        buf[strcspn (buf, "\n")] = '\0';
        numComps = traverse (head, fieldNum, outFile, buf);
        printf ("%s --> %d\n", buf, numComps);
    }
}

/*traverses the tree to look for attribute*/
int traverse (node_t *parent, int fieldNum, FILE *outFile, char *searchQuery)
{
    int comps = 0;
    if (parent == NULL) return 0;

    comps += traverse (parent->lt, fieldNum, outFile, searchQuery);
    comps += searchData (parent->data, fieldNum, outFile, searchQuery);
    comps += traverse (parent->gt, fieldNum, outFile, searchQuery);
    return comps;
}

// insertFromDataList converts the randomised array to a bst
void insertFromDataList (dataList_t *list, node_t *head)
{
    for (int i = 0; i < list->limit; i++)
    {
        if (list->list[i] != NULL)
        {
            bstInsert (head, list->list[i]);
        }
    }
}

void freeDataList (dataList_t *list)
{
    free (list->list);
    list->list = NULL;
    free (list);
    list = NULL;
}

// dataRandomise randomises the list with hash-like behaviour
void dataRandomise (datat *data, dataList_t *list)
{

    // if the list is full, double the space and clear
    if (list->n == list->limit)
    {
        list->list = (datat **)realloc (list->list, sizeof (datat *) * list->limit * 2);
        assert (list->list);

        for (int i = list->n; i < 2 * list->n; i++)
        {
            list->list[i] = NULL;
        }

        list->limit = list->limit * 2;
    }
    // Start at a random starting point, then keep looking for an insert location
    int location = rand () % list->limit;
    while (list->list[location] != NULL)
    {
        location = (location + 1) % (list->limit);
    }
    list->list[location] = data;
    list->n++;
}

dataList_t *dataList ()
{
    dataList_t *d = (dataList_t *)malloc (sizeof (dataList_t));
    d->list = (datat **)calloc (STARTSPACE, sizeof (datat *) * STARTSPACE);
    d->n = 0;
    d->limit = STARTSPACE;
    return d;
}