#include <stdio.h>
#include <stdbool.h>
#define LT -1
#define GT 1
#define EQ 0

#define MAXBUFF 258
#define MAXFIELD 128
#define MAXFIELDNUM 5

// Typedef the key so it could theoreticallybe changed 
typedef int dictKey_t;

/* process is a linked list and has a key, and a 'location', which is the
amount of bytes of data from the start of the input file. When needing to print this value will 
be used to offset the start of the file (ie pointer arithmetic for files)*/

typedef struct page page;
typedef struct mem mem;
typedef struct process process;
typedef struct queue queue;
typedef unsigned int uint;
struct process{
	int key;
	uint arrival;
	uint procid;
	uint jobtime;
	uint loadtime;
	uint page_faults;
	uint remaining;
	uint last_execution_time;
	struct process *llNext;
	struct process *queueNext;
	mem *memory;
};

struct page{
	process* parent;
	bool allocated;
	uint id;
	uint size;
};

struct mem{
	page **pages;
	page **recently_evicted;
	uint num_recently_evicted;
	uint cap;
	uint len;
	uint num_allocated;
};

struct queue{
	process *front, *rear;
	int num;
};


/* parsing.c  */
void fieldFromStr (char *buff, char dest[MAXFIELDNUM][MAXFIELD]);

process*  parseFile(FILE*);

/* processes.c  */
process *newData (char *entry);

void freeData(process*);

process *blankData();

mem *new_memory (uint capacity);

page *new_page ();

/* queue.c */

queue *new_q ();

void add (queue *q, process *d);

process* pop (queue *q);

void add_sorted (queue *q, process *d, bool (*cmp)(process *d, process *t));

