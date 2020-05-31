#include "scheduler.h"
#include <assert.h>
#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char **argv)
{
    char file[200];
    enum scheduler_algorithms algorithm = unset_scheduler;
    enum memory_algorithm memallocation = unset_memory;
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
            else if (strcmp (optarg, "cs") == 0)
            {
                algorithm = custom_schedule;
            }
            break;
        case 'm':
            if (strcmp (optarg, "u") == 0)
            {
                memallocation = unlimited;
            }
            else if (strcmp (optarg, "p") == 0)
            {
                memallocation = swapping;
                quantum = -1;
            }
            else if (strcmp (optarg, "v") == 0)
            {
                memallocation = virtual;
                quantum = -1;
            }
            else if (strcmp (optarg, "cm") == 0)
            {
                memallocation = custom_memory;
                quantum = -1;
            }
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
    process *head = parseFile (inFile);
    run (head, quantum, memsize, memallocation, algorithm);
    return 0;
}
