/*
This code written by Joshua Carpeggiani
data.c contains the data struct and all supporting files
program can be changed to support completely different keys/data entries only by changing the code in this file
*/
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
