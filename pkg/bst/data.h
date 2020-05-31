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

struct process{
	int key;
	int arrival;
	int procid;
	int jobtime;
	int loadtime;
	int page_faults;
	int remaining;
	int finishingtime;
	struct process *llNext;
	struct process *queueNext;
	int last_execution_time;
	mem *memory;
};

typedef struct process process;

struct page{
	process* parent;
	bool allocated;
	int id;
	int size;
};

struct mem{
	page **pages;
	page **recently_evicted;
	int num_recently_evicted;
	int cap;
	int len;
};

struct queue{
	process *front, *rear;
	int num;
};

typedef struct queue queue;
mem *new_memory (int capacity);
dictKey_t *NewKey();
process *newData (char *entry);
process *blankData();
void printData(process*);
void printKey(dictKey_t);
void printFData(process*, FILE*);
void printFAttr(process *d, FILE* outFile, char* Attr);
void fieldFromStr(char *row, char dest[MAXFIELDNUM][MAXFIELD]);
process* linkData(process*, process*);
int searchData(process *,int, FILE*, char *);
void freeData(process*);
process*  parseFile(FILE*);
queue *new_q ();
void add (queue *q, process *d);
process* pop (queue *q);
