#include "../../pkg/schedule/scheduler.h"
#ifndef BST_H
#define BST_H

#include "../../pkg/bst/bst.h"
#endif
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char **argv)
{
    char file[200];
    enum scheduler algorithm;
    int memallocation;
    int memsize;
    int quantum;
    int c;

    while ((c = getopt (argc, argv, "f:a:m:s:q:")) != -1)
        switch (c)
        {
        case 'f':
            strcpy (file, optarg);
            break;
        case 'a':
            if (strcmp (optarg, "rr") == 0)
            {
                algorithm = round_robin;
            }
            else if (strcmp (optarg, "ff") == 0)
            {
                algorithm = first_come;
                quantum = -1;
            }
            break;
        case 'm':
            memallocation = atoi (optarg);
            break;
        case 's':
            memsize = atoi (optarg);
            break;
        case 'q':
            quantum = atoi (optarg);
            break;
        }
    FILE *inFile = fopen (file, "r");
    assert (inFile);
    datat *head = parseFile (inFile);
    next (head, algorithm, quantum);
    return 0;
}