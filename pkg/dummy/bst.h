// #include "data.h"
#include <stdio.h>
#define TRUE 1
#define FALSE 0

#ifndef DATA_H
#define DATA_H

#include "data.h"
#endif

#define NUMSKIPPEDWROWS 0 // number of header rows to skip in input csv
#define MAXBUFF 258
#define MAXFIELD 128
#define MAXFIELDNUM 5
#define STARTSPACE 100

typedef struct node_t {
	struct datat *data;
	struct node_t *lt;
	struct node_t *gt;
}node_t;

node_t *newNode(datat *);
node_t *blankNode();
void bstInsert(node_t *l, datat *);
void freeTree(node_t *);
void printNodes(datat *, FILE*);
void parseKeys(node_t *, FILE*);
node_t*  parseFile(FILE*);
node_t*  fileToHead(FILE*, node_t *);