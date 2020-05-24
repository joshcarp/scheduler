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
#define STARTSPACE 100

typedef struct node_t {
	struct datat *data;
	struct node_t *lt;
	struct node_t *gt;
}node_t;

typedef struct dataList_t{
	datat ** list;
	int n; 
	int limit;
}dataList_t;

node_t *newNode(datat *);
node_t *blankNode();
void bstInsert(node_t *l, datat *);
node_t *searchTree(node_t *, char* );
void freeTree(node_t *);
void printNodes(datat *, FILE*);
void parseKeys(node_t *, FILE*);
node_t*  parseFile(FILE*, int, int);
node_t*  fileToHead(FILE*, node_t *);
void searchTreeAttribute(node_t *, FILE*, int);
int traverse(node_t *, int, FILE*, char*);

void insertFromDataList(dataList_t *, node_t *);
void dataRandomise(datat *, dataList_t *);
dataList_t *dataList();
void freeDataList(dataList_t *);