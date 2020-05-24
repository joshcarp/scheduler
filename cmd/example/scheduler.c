#include "../../pkg/dummy/bst.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char **argv)
{
    char file[200];
    char algorithm[200];
    char memallocation[200];
    char memsize[200];
    char quantum[200];
    int c;

    while ((c = getopt (argc, argv, "f:a:m:s:q:")) != -1)
        switch (c)
        {
        case 'f':
            strcpy (file, optarg);
            break;
        case 'a':
            strcpy (algorithm, optarg);
            break;
        case 'm':
            strcpy (memallocation, optarg);
            break;
        case 's':
            strcpy (memsize, optarg);
            break;
        case 'q':
            strcpy (quantum, optarg);
            break;
        }
    FILE *inFile = fopen (file, "r");
    assert (inFile);
    node_t *head = parseFile (inFile);
    return 0;
}